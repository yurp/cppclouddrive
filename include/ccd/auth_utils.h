
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#include <boost/thread.hpp>

#include <cpprest/http_client.h>
#include <cpprest/oauth2.h>

#include <optional>

namespace ccd::auth_utils
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

boost::future<token> auth_auto(std::string app_id, std::string app_secret, std::string auth_uri, std::string token_uri,
                               std::string redirect_uri, std::string scope);

boost::future<token> auth_refresh(std::string app_id, std::string app_secret, std::string auth_uri,
                                  std::string token_uri, std::string refresh_token);

boost::future<token> auth_manual(std::string app_id, std::string app_secret, std::string auth_uri,
                                 std::string token_uri, std::string redirect_uri, std::string scope);

namespace gdrive
{

boost::future<token> auth_auto(std::string app_id, std::string app_secret, std::string redirect_uri);
boost::future<token> auth_refresh(std::string app_id, std::string app_secret, std::string refresh_token);
boost::future<token> auth_manual(std::string app_id, std::string app_secret);

}

}