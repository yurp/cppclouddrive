
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <cpprest/http_client.h>
#include <cpprest/oauth2.h>

#include <optional>

namespace ccd::auth_utils
{

void save_token(const web::http::oauth2::experimental::oauth2_token& token, const std::string& fname);
web::http::oauth2::experimental::oauth2_token load_token(const std::string& fname);

pplx::task<web::http::client::http_client_config> auth_auto(std::string app_id,
                                                            std::string app_secret,
                                                            std::string auth_uri,
                                                            std::string token_uri,
                                                            std::string redirect_uri,
                                                            std::string scope,
                                                            web::http::client::http_client_config conf = {});

pplx::task<web::http::client::http_client_config> auth_refresh(std::string app_id,
                                                               std::string app_secret,
                                                               std::string auth_uri,
                                                               std::string token_uri,
                                                               std::string refresh_token,
                                                               web::http::client::http_client_config conf = {});

web::http::client::http_client_config auth_access_token(std::string app_id,
                                                        std::string app_secret,
                                                        std::string auth_uri,
                                                        std::string token_uri,
                                                        std::string access_token,
                                                        web::http::client::http_client_config conf = {});

pplx::task<web::http::client::http_client_config> auth_manual(std::string app_id,
                                                              std::string app_secret,
                                                              std::string auth_uri,
                                                              std::string token_uri,
                                                              std::string redirect_uri,
                                                              std::string scope,
                                                              web::http::client::http_client_config conf = {});

namespace gdrive
{
pplx::task<web::http::client::http_client_config> auth_auto(std::string app_id,
                                                            std::string app_secret,
                                                            std::string redirect_uri,
                                                            web::http::client::http_client_config conf = {});
pplx::task<web::http::client::http_client_config> auth_refresh(std::string app_id,
                                                               std::string app_secret,
                                                               std::string refresh_token,
                                                               web::http::client::http_client_config conf = {});
web::http::client::http_client_config auth_access_token(std::string app_id,
                                                        std::string app_secret,
                                                        std::string access_token,
                                                        web::http::client::http_client_config conf = {});
pplx::task<web::http::client::http_client_config> auth_manual(std::string app_id,
                                                              std::string app_secret,
                                                              web::http::client::http_client_config conf = {});
}

}