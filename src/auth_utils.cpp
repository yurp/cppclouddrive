
#include <ccd/auth_utils.h>

#include <cpprest/asyncrt_utils.h>
#include <cpprest/http_listener.h>
#include <cpprest/interopstream.h>

#include <fstream>
#include <mutex>

namespace ccd::auth_utils
{

namespace
{

const utility::string_t root_path = utility::conversions::to_string_t("/");
const utility::string_t access_token_field = utility::conversions::to_string_t("access_token");
const utility::string_t refresh_token_field = utility::conversions::to_string_t("refresh_token");
const utility::string_t token_type_field = utility::conversions::to_string_t("token_type");
const utility::string_t scope_field = utility::conversions::to_string_t("scope");

const std::string gdrive_auth_uri = "https://accounts.google.com/o/oauth2/auth";
const std::string gdrive_token_uri = "https://oauth2.googleapis.com/token";
const std::string gdrive_no_redirect_uri = "urn:ietf:wg:oauth:2.0:oob";
const std::string gdrive_scope = "https://www.googleapis.com/auth/drive";

const std::string dropbox_auth_uri = "https://www.dropbox.com/oauth2/authorize";
const std::string dropbox_token_uri = "https://api.dropboxapi.com/oauth2/token";

web::http::oauth2::experimental::oauth2_config make_oauth2(std::string app_id,
                                                           std::string app_secret,
                                                           std::string auth_uri,
                                                           std::string token_uri,
                                                           std::string redirect_uri = "",
                                                           std::string scope = "")
{
    using namespace utility::conversions;
    return { to_string_t(std::move(app_id)), to_string_t(std::move(app_secret)), to_string_t(std::move(auth_uri)),
             to_string_t(std::move(token_uri)), to_string_t(std::move(redirect_uri)), to_string_t(std::move(scope)) };
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

void save_token(const web::http::oauth2::experimental::oauth2_token& token, const std::string& fname)
{
    if (auto f = std::ofstream { fname })
    {
        auto js = web::json::value::object();
        js[access_token_field] = web::json::value { token.access_token() };
        js[refresh_token_field] = web::json::value { token.refresh_token() };
        js[token_type_field] = web::json::value { token.token_type() };
        js[scope_field] = web::json::value { token.scope() };

        f << utility::conversions::to_utf8string(js.serialize());
    }
}

web::http::oauth2::experimental::oauth2_token load_token(const std::string& fname)
{
    using namespace utility::conversions;

    web::http::oauth2::experimental::oauth2_token token;
    if (auto f = std::ifstream { fname })
    {
        std::string s;
        s.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());

        auto js = web::json::value::parse(to_string_t(s));
        if (js.has_field(access_token_field))
        {
            token.set_access_token(js[access_token_field].as_string());
        }
        if (js.has_field(refresh_token_field))
        {
            token.set_refresh_token(js[refresh_token_field].as_string());
        }
        if (js.has_field(token_type_field))
        {
            token.set_token_type(js[token_type_field].as_string());
        }
        if (js.has_field(scope_field))
        {
            token.set_scope(js[scope_field].as_string());
        }
    }

    return token;
}

pplx::task<web::http::client::http_client_config> auth_auto(std::string app_id,
                                                            std::string app_secret,
                                                            std::string auth_uri,
                                                            std::string token_uri,
                                                            std::string redirect_uri,
                                                            std::string scope,
                                                            web::http::client::http_client_config conf)
{
    using namespace web::http;

    conf.set_oauth2(make_oauth2(std::move(app_id), std::move(app_secret), std::move(auth_uri), std::move(token_uri),
                                std::move(redirect_uri), std::move(scope)));
    auto oauth2_conf = conf.oauth2();

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

    return listener->open().then([oauth2_conf, redirected_uri_task = pplx::create_task(redirected_uri_event)]
    {
        open_browser(oauth2_conf->build_authorization_uri(true));
        return redirected_uri_task;
    })
    .then([oauth2_conf](web::uri redirected_uri)
    {
        return oauth2_conf->token_from_redirected_uri(redirected_uri);
    })
    .then([listener]
    {
        return listener->close();
    })
    .then([conf = std::move(conf), listener]
    {
        return conf;
    });
}

pplx::task<web::http::client::http_client_config> auth_refresh(std::string app_id,
                                                               std::string app_secret,
                                                               std::string auth_uri,
                                                               std::string token_uri,
                                                               std::string refresh_token,
                                                               web::http::client::http_client_config conf)
{
    conf.set_oauth2(make_oauth2(std::move(app_id), std::move(app_secret), std::move(auth_uri), std::move(token_uri)));
    auto oauth2_conf = conf.oauth2();
    web::http::oauth2::experimental::oauth2_token t;
    t.set_refresh_token(refresh_token);
    oauth2_conf->set_token(std::move(t));
    return oauth2_conf->token_from_refresh().then([conf = std::move(conf)]
    {
        return conf;
    });
}

web::http::client::http_client_config auth_access_token(std::string app_id,
                                                        std::string app_secret,
                                                        std::string auth_uri,
                                                        std::string token_uri,
                                                        std::string access_token,
                                                        web::http::client::http_client_config conf)
{
    conf.set_oauth2(make_oauth2(std::move(app_id), std::move(app_secret), std::move(auth_uri), std::move(token_uri)));
    conf.oauth2()->set_token(std::move(access_token));

    return conf;
}

pplx::task<web::http::client::http_client_config> auth_manual(std::string app_id,
                                                              std::string app_secret,
                                                              std::string auth_uri,
                                                              std::string token_uri,
                                                              std::string redirect_uri,
                                                              std::string scope,
                                                              web::http::client::http_client_config conf)
{
    using namespace utility::conversions;

    conf.set_oauth2(make_oauth2(std::move(app_id), std::move(app_secret), std::move(auth_uri), std::move(token_uri),
                                std::move(redirect_uri), std::move(scope)));
    auto oauth2_conf = conf.oauth2();

    std::cout << "go to:" << to_utf8string(oauth2_conf->build_authorization_uri(true)) << "\n"
              << "then paste input code here: ";

    auto code_buf = std::make_shared<Concurrency::streams::stringstreambuf>();
    return acin().read_line(*code_buf).then([code_buf, oauth2_conf](size_t)
    {
        return oauth2_conf->token_from_code(code_buf->collection());
    })
    .then([conf = std::move(conf)]
    {
        return conf;
    });
}

namespace gdrive
{

pplx::task<web::http::client::http_client_config> auth_auto(std::string app_id,
                                                            std::string app_secret,
                                                            std::string redirect_uri,
                                                            web::http::client::http_client_config conf)
{
    return ccd::auth_utils::auth_auto(std::move(app_id), std::move(app_secret), gdrive_auth_uri, gdrive_token_uri,
                                      std::move(redirect_uri), gdrive_scope, std::move(conf));
}

pplx::task<web::http::client::http_client_config> auth_refresh(std::string app_id,
                                                               std::string app_secret,
                                                               std::string refresh_token,
                                                               web::http::client::http_client_config conf)
{
    return ccd::auth_utils::auth_refresh(std::move(app_id), std::move(app_secret), gdrive_auth_uri, gdrive_token_uri,
                                         std::move(refresh_token), std::move(conf));
}

web::http::client::http_client_config auth_access_token(std::string app_id,
                                                        std::string app_secret,
                                                        std::string access_token,
                                                        web::http::client::http_client_config conf)
{
    return ccd::auth_utils::auth_access_token(std::move(app_id), std::move(app_secret), gdrive_auth_uri,
                                              gdrive_token_uri, std::move(access_token), std::move(conf));
}

pplx::task<web::http::client::http_client_config> auth_manual(std::string app_id,
                                                              std::string app_secret,
                                                              web::http::client::http_client_config conf)
{
    return ccd::auth_utils::auth_manual(std::move(app_id), std::move(app_secret), gdrive_auth_uri, gdrive_token_uri,
                                        gdrive_no_redirect_uri, gdrive_scope, std::move(conf));
}

}

}