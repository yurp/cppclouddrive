
#include <ccd/auth_utils.h>
#include <ccd/dropbox/resource/dropbox_list_folder.h>

#include <iostream>

// set your app id here or define outside
#ifndef DROPBOX_APP_ID
#define DROPBOX_APP_ID ""
#endif

// set your app's secret key here or define outside
#ifndef DROPBOX_SECRET_KEY
#define DROPBOX_SECRET_KEY ""
#endif

pplx::task<web::http::client::http_client_config> auth()
{
    using namespace ccd::auth_utils;
    using namespace utility::conversions;
    using namespace web::http::client;

    std::string token_file = "token.json";
    std::string redirect_uri = "http://localhost:25000/";

    auto token = ccd::auth_utils::load_token(token_file);
    if (!token.access_token().empty())
    {
        return pplx::task_from_result(dropbox::auth_access_token(DROPBOX_APP_ID, DROPBOX_SECRET_KEY,
                                                                 to_utf8string(token.access_token())));
    }

    return dropbox::auth_auto(DROPBOX_APP_ID, DROPBOX_SECRET_KEY, redirect_uri).then([token_file](http_client_config c)
    {
        ccd::auth_utils::save_token(c.oauth2()->token(), token_file);
        return c;
    });
}

int main()
{
    auto client = auth().then([](web::http::client::http_client_config c)
    {
        return std::make_shared<web::http::client::http_client>("https://api.dropboxapi.com", c);
    });

    auto mdlst = ccd::dropbox::resource::files::list_folder{ client, "" }.set_recursive(true)
                                                                         .set_include_deleted(true)
                                                                         .set_include_media_info(true)
                                                                         .set_limit(999)
                                                                         .set_include_has_explicit_shared_members(false)
                                                                         .exec().get();
    while (true)
    {
        std::cerr << "-------\n";
        if (auto md = mdlst.get_entries())
        {
            for (const auto& i: *md)
            {
                std::cerr << i.get_path_display().value_or("?")
                          << "\t[" << i.get_id().value_or("?") << "]"
                          << ", " << i.get_size().value_or(-1)
                          << "\n";
            }
        }

        if (!mdlst.get_has_more().value_or(false))
            break;

        mdlst = ccd::dropbox::resource::files::list_folder_continue{ client, *mdlst.get_cursor() }.exec().get();
    }

    return 0;
}
