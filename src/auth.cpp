
#include <ccd/auth.h>

#include <ccd/http/beast_transport.h>
#include <ccd/utils.h>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>

#include <fstream>
#include <iostream>
#include <mutex>

namespace ccd::auth
{

namespace
{

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

const std::string root_path = "/";
const std::string access_token_field = "access_token";
const std::string refresh_token_field = "refresh_token";
const std::string token_type_field = "token_type";
const std::string scope_field = "scope";

const std::string gdrive_auth_uri = "https://accounts.google.com/o/oauth2/auth";
const std::string gdrive_token_uri = "https://oauth2.googleapis.com/token";
const std::string gdrive_no_redirect_uri = "urn:ietf:wg:oauth:2.0:oob";
const std::string gdrive_scope = "https://www.googleapis.com/auth/drive";

const std::string dropbox_auth_uri = "https://www.dropbox.com/oauth2/authorize";
const std::string dropbox_token_uri = "https://api.dropboxapi.com/oauth2/token";

std::pair<std::string, http::response<http::string_body>> process_request(const http::request<http::string_body>& req)
{
    std::string out;
    auto st = http::status::not_found;
    auto body = "NOT FOUND";
    auto pc = req.target().find("code=");
    if (pc != boost::string_view::npos)
    {
        auto pce = req.target().find("&", pc);
        auto len = (pce == boost::string_view::npos) ? pce : (pce - (pc + 5));
        st = http::status::ok;
        out = std::string{ req.target().substr(pc + 5, len) };
        body = "OK";
    }

    http::response<http::string_body> res { st, req.version() };
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/html");
    res.body() = body;
    res.prepare_payload();

    return { std::move(out), std::move(res) };
}

std::pair<boost::future<void>, boost::future<std::string>> code_from_listener(std::string redirect_uri)
{
    auto p = redirect_uri.find(':', 8);
    unsigned short port = (p == std::string::npos) ? 0 : std::atoi(redirect_uri.c_str() + p + 1);

    boost::promise<void> started_promise;
    auto started = started_promise.get_future();
    auto finished = boost::async(boost::launch::async, [started_promise = std::move(started_promise), port]() mutable
    {
        net::io_context ioc{ 1 };
        tcp::acceptor acceptor{ ioc, { net::ip::make_address("0.0.0.0"), port } };
        std::string out;
        started_promise.set_value();
        while (out.empty())
        {
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            beast::error_code ec;
            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req, ec);
            if(!ec)
            {
                auto res = process_request(req);
                out = std::move(res.first);

                http::serializer<false, http::string_body> sr { res.second };
                http::write(socket, sr, ec);
            }
            if(ec)
            {
                throw boost::system::system_error{ ec };
            }

            socket.shutdown(tcp::socket::shutdown_send, ec);
        }

        return out;
    });

    return { std::move(started), std::move(finished) };
}


oauth2::token to_token(std::string s)
{
    oauth2::token t;

    auto js = ccd::from_json(s);
    if (js.is<var::map_t>())
    {
        if (js.has(access_token_field))
        {
            t.access = js[access_token_field].as<std::string>();
        }
        if (js.has(refresh_token_field))
        {
            t.refresh = js[refresh_token_field].as<std::string>();
        }
        if (js.has(token_type_field))
        {
            t.type = js[token_type_field].as<std::string>();
        }
        if (js.has(scope_field))
        {
            t.scope = js[scope_field].as<std::string>();
        }
    }

    return t;
}

oauth2::token response_future_to_token(ccd::http::response_future f)
{
    return to_token(std::move(f.get().body));
}

std::string generate_auth_uri(std::string app_id,
                              std::string auth_uri,
                              std::optional<std::string> redirect_uri,
                              std::optional<std::string> scope)
{
    std::string uri = auth_uri + "?response_type=code";
    uri += "&client_id=" + app_id;
    uri += "&state=1234567890";
    if (redirect_uri)
    {
        uri += "&redirect_uri=" + *redirect_uri;
    }
    if (scope)
    {
        uri += "&scope=" + *scope;
    }

    return uri;
}


boost::future<oauth2::token> token_from_code(const std::string& code,
                                             const std::string& app_id,
                                             const std::string& app_secret,
                                             const std::string token_host,
                                             const std::string& token_path,
                                             std::optional<std::string> redirect_uri)
{
    ccd::http::request r;
    r.method = "POST";
    r.host = token_host;
    r.path = token_path;
    r.content_type = "application/x-www-form-urlencoded";
    r.body  = "grant_type=authorization_code";
    r.body += "&code=" + code;
    r.body += "&client_id=" + app_id;
    r.body += "&client_secret=" + app_secret;
    if (redirect_uri)
    {
        r.body += "&redirect_uri=" + *redirect_uri;
    }

    return ccd::http::beast_transport_factory()(r).then(response_future_to_token);
}

void open_browser(const std::string& auth_uri)
{
#if defined(_WIN32) && !defined(__cplusplus_winrt)
    // NOTE: Windows desktop only.
    auto r = ShellExecuteA(NULL, "open", auth_uri.c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif defined(__APPLE__)
    // NOTE: OS X only.
    auto browser_cmd = "open \"" + auth_uri + "\"";
    (void)system(browser_cmd.c_str());
#else
    // NOTE: Linux/X11 only.
    auto browser_cmd = "xdg-open \"" + auth_uri + "\"";
    (void)system(browser_cmd.c_str());
#endif
}

}

oauth2::oauth2(std::string app_id, std::string app_secret, std::string auth_uri, std::string token_uri)
    : m_app_id(std::move(app_id))
    , m_app_secret(std::move(app_secret))
    , m_auth_uri(std::move(auth_uri))
{
    auto p = token_uri.find('/', 8);
    m_token_host = token_uri.substr(0, p);
    m_token_path = token_uri.substr(p);
}

void oauth2::save_token(const oauth2::token& t, const std::string& fname)
{
    if (auto f = std::ofstream { fname })
    {
        ccd::var js { ccd::var::map_t { { access_token_field, t.access },
                                        { access_token_field, t.refresh },
                                        { access_token_field, t.type },
                                        { access_token_field, t.scope }, }};

        f << ccd::to_json(js);
    }
}

oauth2::token oauth2::load_token(const std::string& fname)
{
    if (auto f = std::ifstream { fname })
    {
        return to_token({ std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>() });
    }

    return oauth2::token { };
}

boost::future<oauth2::token> oauth2::automatic(const std::string& redirect_uri,
                                               const std::optional<std::string>& scope)
{
    auto auth_args = std::make_tuple(m_app_id, m_auth_uri, redirect_uri, scope);
    auto token_args = std::make_tuple(std::string{}, m_app_id, m_app_secret, m_token_host, m_token_path, redirect_uri);

    auto [ start, finish ] = code_from_listener(redirect_uri);
    return start.then([auth_args = std::move(auth_args), finish = std::move(finish)](boost::future<void> ) mutable
    {
        open_browser(std::apply(generate_auth_uri, std::move(auth_args)));
        return std::move(finish);
    })
    .unwrap().then([token_args = std::move(token_args)](boost::future<std::string> code) mutable
    {
        std::get<0>(token_args) = code.get();
        return std::apply(token_from_code, std::move(token_args));
    })
    .unwrap();
}

boost::future<oauth2::token> oauth2::manual(const std::optional<std::string>& redirect_uri,
                                            const std::optional<std::string>& scope)
{
    std::cout << "go to: " << generate_auth_uri(m_app_id, m_auth_uri, redirect_uri, scope) << "\n"
              << "then paste input code here: ";

    auto token_args = std::make_tuple(std::string{}, m_app_id, m_app_secret, m_token_host, m_token_path, redirect_uri);
    return boost::async(boost::launch::async, []
    {
        std::string code;
        std::cin >> code;
        return code;
    })
    .then([token_args = std::move(token_args), redirect_uri](boost::future<std::string> code) mutable
    {
        std::get<0>(token_args) = code.get();
        return std::apply(token_from_code, std::move(token_args));
    })
    .unwrap();
}

boost::future<oauth2::token> oauth2::refresh(const std::string& refresh_token)
{
    ccd::http::request r;
    r.method = "POST";
    r.host = m_token_host;
    r.path = m_token_path;
    r.content_type = "application/x-www-form-urlencoded";
    r.body = "grant_type=refresh_token";
    r.body += "&refresh_token=" + refresh_token;
    r.body += "&client_id=" + m_app_id;
    r.body += "&client_secret=" + m_app_secret;

    return ccd::http::beast_transport_factory()(r).then(response_future_to_token);
}

oauth2_gdrive::oauth2_gdrive(std::string app_id, std::string app_secret)
    : m_oauth2(std::move(app_id), std::move(app_secret), gdrive_auth_uri, gdrive_token_uri)
{
}

boost::future<oauth2::token> oauth2_gdrive::automatic(const std::string& redirect_uri)
{
    return m_oauth2.automatic(redirect_uri, gdrive_scope);
}

boost::future<oauth2::token> oauth2_gdrive::manual()
{
    return m_oauth2.manual(gdrive_no_redirect_uri, gdrive_scope);
}

boost::future<oauth2::token> oauth2_gdrive::refresh(const std::string& refresh_token)
{
    return m_oauth2.refresh(refresh_token);
}

oauth2_dropbox::oauth2_dropbox(std::string app_id, std::string app_secret)
    : m_oauth2(std::move(app_id), std::move(app_secret), dropbox_auth_uri, dropbox_token_uri)
{
}

boost::future<oauth2::token> oauth2_dropbox::automatic(const std::string& redirect_uri)
{
    return m_oauth2.automatic(redirect_uri, "");
}

boost::future<oauth2::token> oauth2_dropbox::manual()
{
    return m_oauth2.manual(std::nullopt, std::nullopt);
}

boost::future<oauth2::token> oauth2_dropbox::refresh(const std::string& refresh_token)
{
    return m_oauth2.refresh(refresh_token);
}

}
