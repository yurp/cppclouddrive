
#include <ccd/auth_utils.h>
#include <ccd/dropbox/dropbox.h>

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
    ccd::dropbox::dropbox d { auth() };

    auto mdlst = d.files_resource().list_folder_request("/tmp")
                                   .set_recursive(true)
                                   .set_include_deleted(true)
                                   .set_include_media_info(true)
                                   .set_limit(999)
                                   .exec().get();
    while (true)
    {
        std::cerr << "-------\n";
        if (auto md = mdlst.get_entries())
        {
            for (auto& i: *md)
            {
                if (auto mi = i.get_media_info())
                {
                    mi->set_time_taken("200-01-01T00:00:00Z");
                    i.set_media_info(mi);
                }

                std::cerr << i.to_json().serialize() << "\n";
                //std::cerr << i.get_path_display().value_or("?")
                //          << "\t[" << i.get_id().value_or("?") << "]"
                //          << ", " << i.get_size().value_or(-1)
                //          << "\n";
            }
        }

        if (!mdlst.get_has_more().value_or(false))
            break;

        mdlst = d.files_resource().list_folder_continue_request(*mdlst.get_cursor()).exec().get();
    }

    return 0;
}
