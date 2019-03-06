
#include <ccd/http/curl_transport.h>

#include <curl/curl.h>

#include <iostream>

#include <mutex>
#include <thread>

namespace ccd::http
{
#if 0
namespace
{

bool is_running = true;
std::once_flag f;
CURLM* multi_handle = nullptr;
std::thread t;
std::mutex m;

void thread_loop()
{
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);
    while (is_running)
    {
        // set a suitable timeout to play around with ------------------------------------------------------------------
        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        long curl_timeo = -1;
        curl_multi_timeout(multi_handle, &curl_timeo);
        if(curl_timeo >= 0)
        {
            timeout.tv_sec = curl_timeo / 1000;
            if(timeout.tv_sec > 1)
            {
                timeout.tv_sec = 1;
            }
            else
            {
                timeout.tv_usec = (curl_timeo % 1000) * 1000;
            }
        }

        // get file descriptors from the transfers ---------------------------------------------------------------------
        fd_set fdread;
        fd_set fdwrite;
        fd_set fdexcep;

        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcep);

        int maxfd = -1;

        CURLMcode mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

        if(mc != CURLM_OK)
        {
            std::cerr << "curl_multi_fdset() failed, code " << mc << "\n";
            break;
        }

        int rc; /* select() return code */
        if(maxfd == -1)
        {
#ifdef _WIN32
            Sleep(100);
            rc = 0;
#else
            /* Portable sleep for platforms other than Windows. */
            struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
            rc = select(0, NULL, NULL, NULL, &wait);
#endif
        }
        else
        {
            /* Note that on some platforms 'timeout' may be modified by select().
               If you need access to the original value save a copy beforehand. */
            rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
        }

        switch(rc)
        {
        case -1:
            /* select error */
            break;
        case 0: /* timeout */
        default: /* action */
            curl_multi_perform(multi_handle, &still_running);
            break;
        }
    }
}

}

response_future curl_transport(request r)
{
    auto h = curl_easy_init();
    curl_easy_setopt(h, CURLOPT_URL, "https://example.com");
    return response_future{};
}

transport_func curl_transport_factory()
{
    std::call_once(f, []
    {
        multi_handle = curl_multi_init();
        auto t = std::thread(thread_loop);
        t.detach();
    });

    return curl_transport;
}
#endif

namespace
{

std::once_flag f;

std::string encode(CURL* curl, const std::string& s)
{
    auto es = curl_easy_escape(curl, s.c_str(), s.length());
    std::string out = es;
    if (es)
    {
        curl_free(es);
    }

    return out;
}

std::string build_url(CURL* curl, const request& r)
{
    std::string url = r.host + "/" + r.path;
    char c = '?';
    for (const auto& q: r.queries)
    {
        url += c;
        c = '&';
        url += encode(curl, q.first) + "=" + encode(curl, q.second);
    }

    return url;
}

size_t write_cb(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    auto resp = reinterpret_cast<ccd::http::response*>(userp);

    auto s = reinterpret_cast<const char*>(contents);
    resp->body += std::string(s, s + realsize);

    return realsize;
}

}

class curl_transport
{
public:
    curl_transport() {}
    curl_transport(curl_transport&& other) : m_threads(std::move(other.m_threads)) { }
    curl_transport(const curl_transport& ) { }

    response_future operator()(request r)
    {
        boost::promise<ccd::http::response> p;
        auto f = p.get_future();
        m_threads.emplace_back([p = std::move(p), r = std::move(r)]() mutable
        {
            auto curl = curl_easy_init();
            if (curl)
            {
                curl_easy_setopt(curl, CURLOPT_URL, build_url(curl, r).c_str());

                curl_slist* headers = nullptr;
                for (const auto& h: r.headers)
                {
                    auto x = h.first + ": " + h.second;
                    headers = curl_slist_append(headers, x.c_str());
                }

                if (!r.body.empty())
                {
                    auto type = "Content-Type: " + r.content_type;
                    headers = curl_slist_append(headers, type.c_str());
                    //auto len = "Content-Length: " + std::to_string(r.body.length());
                    //headers = curl_slist_append(headers, len.c_str());
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, r.body.length());
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, r.body.c_str());
                }

                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, r.method.c_str());
                curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

                ccd::http::response resp { 200, {} };
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);

                curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                auto res = curl_easy_perform(curl);
                if(res != CURLE_OK)
                {
                    std::cerr << "curl err: " << curl_easy_strerror(res);
                }

                curl_slist_free_all(headers);
                curl_easy_cleanup(curl);

                p.set_value(std::move(resp));
            }
        });

        return f;
    }

    ~curl_transport()
    {
        for (auto& t: m_threads)
            if (t.joinable())
                t.join();
    }

private:
    std::vector<std::thread> m_threads;
};

transport_func curl_transport_factory()
{
    std::call_once(f, []
    {
        curl_global_init(CURL_GLOBAL_ALL);
    });

    return curl_transport {};
}

}
