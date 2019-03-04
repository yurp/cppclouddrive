
#include <ccd/http/curl_transport.h>

#include <curl/curl.h>

#include <iostream>

#include <mutex>
#include <thread>

namespace ccd::http
{

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

}
