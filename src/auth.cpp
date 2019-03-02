
#include <ccd/auth.h>

#include <ccd/utils.h>

#include <cpprest/asyncrt_utils.h>
#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include <cpprest/interopstream.h>
#include <cpprest/oauth2.h>

#include <fstream>
#include <mutex>

namespace ccd::auth::oauth2
{

namespace
{

const std::string root_path = utility::conversions::to_string_t("/");
const std::string access_token_field = utility::conversions::to_string_t("access_token");
const std::string refresh_token_field = utility::conversions::to_string_t("refresh_token");
const std::string token_type_field = utility::conversions::to_string_t("token_type");
const std::string scope_field = utility::conversions::to_string_t("scope");

const std::string gdrive_auth_uri = "https://accounts.google.com/o/oauth2/auth";
const std::string gdrive_token_uri = "https://oauth2.googleapis.com/token";
const std::string gdrive_no_redirect_uri = "urn:ietf:wg:oauth:2.0:oob";
const std::string gdrive_scope = "https://www.googleapis.com/auth/drive";

const std::string dropbox_auth_uri = "https://www.dropbox.com/oauth2/authorize";
const std::string dropbox_token_uri = "https://api.dropboxapi.com/oauth2/token";

std::shared_ptr<web::http::oauth2::experimental::oauth2_config> make_oauth2(std::string app_id,
                                                                            std::string app_secret,
                                                                            std::string auth_uri,
                                                                            std::string token_uri,
                                                                            std::string redirect_uri = "",
                                                                            std::string scope = "")
{
    using namespace utility::conversions;
    using namespace web::http::oauth2::experimental;
    return std::make_shared<oauth2_config>(to_string_t(std::move(app_id)), to_string_t(std::move(app_secret)),
                                           to_string_t(std::move(auth_uri)), to_string_t(std::move(token_uri)),
                                           to_string_t(std::move(redirect_uri)), to_string_t(std::move(scope)));
}

token to_token(const web::http::oauth2::experimental::oauth2_config& c)
{
    using namespace utility::conversions;
    const auto& ot = c.token();
    return { to_utf8string(ot.access_token()),
             to_utf8string(ot.refresh_token()),
             to_utf8string(ot.token_type()),
             to_utf8string(ot.scope()) };
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
    string_t browser_cmd(U("xdg-open \"") + auth_uri + U("\""));
    (void)system(browser_cmd.c_str());
#endif
}

Concurrency::streams::stdio_istream<char>& acin()
{
    static Concurrency::streams::stdio_istream<char> is { std::cin };
    return is;
}

}

void save_token(const token& t, const std::string& fname)
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

token load_token(const std::string& fname)
{
    token t;
    if (auto f = std::ifstream { fname })
    {
        std::string s;
        s.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());

        auto js = ccd::from_json(s);

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

boost::future<token> automatic(std::string app_id, std::string app_secret, std::string auth_uri, std::string token_uri,
                               std::string redirect_uri, std::string scope)
{
    using namespace web::http;

    auto oauth2_conf = make_oauth2(std::move(app_id), std::move(app_secret), std::move(auth_uri), std::move(token_uri),
                                   std::move(redirect_uri), std::move(scope));

    pplx::task_completion_event<web::uri> redirected_uri_event;
    auto listener = std::make_shared<experimental::listener::http_listener>(oauth2_conf->redirect_uri());
    listener->support([redirected_uri_event, done = false, m = std::make_shared<std::mutex>()](http_request r) mutable
    {
        std::lock_guard<std::mutex> lck(*m);
        if (!done && r.request_uri().path() == root_path && !r.request_uri().query().empty())
        {
            r.reply(web::http::status_codes::OK);
            redirected_uri_event.set(r.request_uri());
            done = true;
            return;
        }
        r.reply(web::http::status_codes::NotFound);
    });

    boost::promise<token> p;
    auto f = p.get_future();

    listener->open().then([oauth2_conf, redirected_uri_task = pplx::create_task(redirected_uri_event)]
    {
        open_browser(oauth2_conf->build_authorization_uri(true));
        return redirected_uri_task;
    })
    .then([oauth2_conf](web::uri redirected_uri) { return oauth2_conf->token_from_redirected_uri(redirected_uri); })
    .then([listener]                             { return listener->close(); })
    .then([listener]                             { return 0; })
    .then([oauth2_conf, p = std::move(p)](pplx::task<int> i) mutable
    {
        try
        {
            i.get(); // if exception occured it will throw here
            p.set_value(to_token(*oauth2_conf));
        }
        catch (...)
        {
            p.set_exception(std::current_exception()); // TODO: looks like set_exception() can throw
        }
    });

    return f;
}

boost::future<token> manual(std::string app_id, std::string app_secret, std::string auth_uri, std::string token_uri,
                            std::string redirect_uri, std::string scope)
{
    using namespace utility::conversions;

    auto oauth2_conf = make_oauth2(std::move(app_id), std::move(app_secret), std::move(auth_uri), std::move(token_uri),
                                   std::move(redirect_uri), std::move(scope));

    std::cout << "go to:" << to_utf8string(oauth2_conf->build_authorization_uri(true)) << "\n"
              << "then paste input code here: ";

    boost::promise<token> p;
    auto f = p.get_future();

    auto code_buf = std::make_shared<Concurrency::streams::stringstreambuf>();
    acin().read_line(*code_buf).then([code_buf, oauth2_conf](size_t)
    {
        return oauth2_conf->token_from_code(code_buf->collection());
    })
    .then([]
    {
        return 0;
    })
    .then([oauth2_conf, p = std::move(p)](pplx::task<int> i) mutable
    {
        try
        {
            i.get(); // if exception occured it will throw here
            p.set_value(to_token(*oauth2_conf));
        }
        catch (...)
        {
            p.set_exception(std::current_exception()); // TODO: looks like set_exception() can throw
        }
    });

    return f;
}

boost::future<token> refresh(std::string app_id, std::string app_secret, std::string auth_uri, std::string token_uri,
                             std::string refresh_token)
{
    auto oauth2_conf = make_oauth2(std::move(app_id), std::move(app_secret), std::move(auth_uri), std::move(token_uri));
    web::http::oauth2::experimental::oauth2_token t;
    t.set_refresh_token(utility::conversions::to_string_t(refresh_token));
    oauth2_conf->set_token(std::move(t));

    boost::promise<token> p;
    auto f = p.get_future();

    oauth2_conf->token_from_refresh().then([]
    {
        return 0;
    })
    .then([oauth2_conf, p = std::move(p)](pplx::task<int> i) mutable
    {
        try
        {
            i.get(); // if exception occured it will throw here
            p.set_value(to_token(*oauth2_conf));
        }
        catch (...)
        {
            p.set_exception(std::current_exception()); // TODO: looks like set_exception() can throw
        }
    });

    return f;
}

namespace gdrive
{

boost::future<token> automatic(std::string app_id, std::string app_secret, std::string redirect_uri)
{
    return ccd::auth::oauth2::automatic(std::move(app_id), std::move(app_secret), gdrive_auth_uri, gdrive_token_uri,
                                        std::move(redirect_uri), gdrive_scope);
}

boost::future<token> manual(std::string app_id, std::string app_secret)
{
    return ccd::auth::oauth2::manual(std::move(app_id), std::move(app_secret), gdrive_auth_uri, gdrive_token_uri,
                                     gdrive_no_redirect_uri, gdrive_scope);
}

boost::future<token> refresh(std::string app_id, std::string app_secret, std::string refresh_token)
{
    return ccd::auth::oauth2::refresh(std::move(app_id), std::move(app_secret), gdrive_auth_uri, gdrive_token_uri,
                                      std::move(refresh_token));
}

}

}