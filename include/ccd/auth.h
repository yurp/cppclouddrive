
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#include <boost/thread.hpp>

namespace ccd::auth::oauth2
{

struct token
{
    std::string access;
    std::string refresh;
    std::string type;
    std::string scope;
};

void save_token(const token& t, const std::string& fname);
token load_token(const std::string& fname);

boost::future<token> automatic(std::string app_id,
                               std::string app_secret,
                               std::string auth_uri,
                               std::string token_uri,
                               std::string redirect_uri,
                               std::string scope);

boost::future<token> manual(std::string app_id,
                            std::string app_secret,
                            std::string auth_uri,
                            std::string token_uri,
                            std::string redirect_uri,
                            std::string scope);

boost::future<token> refresh(std::string app_id,
                             std::string app_secret,
                             std::string auth_uri,
                             std::string token_uri,
                             std::string refresh_token);

namespace gdrive
{

boost::future<token> automatic(std::string app_id, std::string app_secret, std::string redirect_uri);
boost::future<token> manual(std::string app_id, std::string app_secret);
boost::future<token> refresh(std::string app_id, std::string app_secret, std::string refresh_token);

}

}
