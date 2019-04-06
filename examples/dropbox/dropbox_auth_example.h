
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/auth.h>
#include <ccd/http/beast_transport.h>

inline boost::future<ccd::auth::oauth2::token> auth(boost::asio::io_service& ios)
{
    std::string token_file = "dropbox_token.json";
    auto oa2token = ccd::auth::oauth2::load_token(token_file);
    if (!oa2token.access.empty())
    {
        return boost::make_ready_future(oa2token);
    }

    auto redirect_uri = "http://localhost:25000/";
    auto app_id = std::getenv("DROPBOX_APP_ID");
    auto app_secret = std::getenv("DROPBOX_SECRET_KEY");
    if (app_id == nullptr || app_secret == nullptr)
    {
        boost::throw_exception(std::runtime_error{ "app credentials aren't set" });
    }

    ccd::auth::oauth2_dropbox oa2 { ccd::http::async_beast_transport_factory(ios), app_id, app_secret };
    return oa2.automatic(redirect_uri).then([token_file](boost::future<ccd::auth::oauth2::token> ft)
    {
        auto t = ft.get();
        ccd::auth::oauth2::patch_token(t, token_file);
        return t;
    });
}
