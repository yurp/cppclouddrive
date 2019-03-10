
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/defs.h>

namespace ccd::auth
{

class oauth2
{
public:
    struct token
    {
        std::string access;
        std::string refresh;
        std::string type;
        std::string scope;
    };

    static void save_token(const token& t, const std::string& fname);
    static token load_token(const std::string& fname);
    static void patch_token(const token& t, const std::string& fname);

    oauth2(std::string app_id, std::string app_secret, std::string auth_uri, std::string token_uri);

    boost::future<token> automatic(const std::string& redirect_uri, const std::optional<std::string>& scope);

    boost::future<token> manual(const std::optional<std::string>& redirect_uri,
                                const std::optional<std::string>& scope);

    boost::future<token> refresh(const std::string& refresh_token);

private:
    std::string m_app_id;
    std::string m_app_secret;
    std::string m_auth_uri;
    std::string m_token_host;
    std::string m_token_path;
};

class oauth2_gdrive
{
public:
    oauth2_gdrive(std::string app_id, std::string app_secret);

    boost::future<oauth2::token> automatic(const std::string& redirect_uri);
    boost::future<oauth2::token> manual();
    boost::future<oauth2::token> refresh(const std::string& refresh_token);

private:
    oauth2 m_oauth2;

};

class oauth2_dropbox
{
public:
    oauth2_dropbox(std::string app_id, std::string app_secret);

    boost::future<oauth2::token> automatic(const std::string& redirect_uri);
    boost::future<oauth2::token> manual();
    boost::future<oauth2::token> refresh(const std::string& refresh_token);

private:
    oauth2 m_oauth2;

};

}
