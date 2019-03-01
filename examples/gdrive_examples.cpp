
#include <ccd/auth_utils.h>
#include <ccd/gdrive/gdrive.h>

#include <iostream>

// set your app id here or define outside
#ifndef GDRIVE_APP_ID
#define GDRIVE_APP_ID ""
#endif

// set your app's secret key here or define outside
#ifndef GDRIVE_SECRET_KEY
#define GDRIVE_SECRET_KEY ""
#endif

boost::future<ccd::auth_utils::token> auth()
{
    using namespace ccd::auth_utils;
    using namespace utility::conversions;
    using namespace web::http::client;

    std::string token_file = "token.json";
    std::string redirect_uri = "http://localhost:25000/";

    auto token = ccd::auth_utils::load_token(token_file);
    if (!token.refresh.empty())
    {
        return gdrive::auth_refresh(GDRIVE_APP_ID, GDRIVE_SECRET_KEY, token.refresh);
    }

    if (!token.access.empty())
    {
        return boost::make_ready_future(token);
    }

    return gdrive::auth_auto(GDRIVE_APP_ID, GDRIVE_SECRET_KEY, redirect_uri)
           .then([token_file](boost::future<ccd::auth_utils::token> tf)
    {
        auto t = tf.get();
        ccd::auth_utils::save_token(t, token_file);
        return t;
    });
}

void list_root_dir(ccd::gdrive::v3::resource::files::files& files_rsc)
{
    auto file_list = files_rsc.list_request().set_page_size(999)
                                             .set_fields("files(id,name,mimeType,size,createdTime)")
                                             .set_q("'root' in parents")
                                             .exec().get();

    if (auto files = file_list.get_files())
    {
        std::cout << "gdrive root directory:\n";
        for (const auto& file: *files)
        {
            std::cout << "id = " << file.get_id().value_or("?")
                      << ", name = " << file.get_name().value_or("?")
                      << ", mime type = " << file.get_mime_type().value_or("?")
                      << ", size = " << file.get_size().value_or(-1)
                      << ", created = " << file.get_created_time().value_or("?") << "\n";
        }
    }
}

void print_getting_started_content(ccd::gdrive::v3::resource::files::files& files_rsc)
{
    auto file_list = files_rsc.list_request().set_page_size(1)
                                             .set_fields("files(id,mimeType)")
                                             .set_q("'root' in parents and name='Getting started'")
                                             .exec().get();

    auto files = file_list.get_files();
    if (!files || files->empty())
    {
        std::cerr << "responce doesn't contain files\n";
        return;
    }

    const auto& f = (*files)[0];
    auto id = f.get_id();
    if (!id || f.get_mime_type().value_or("") != "application/pdf")
    {
        std::cerr << "Getting started PDF has not been found\n";
        return;
    }

    auto content = files_rsc.get_request(*id).exec_media(/*offset=*/0, /*size=*/128).get();
    std::cout << "Getting started pdf:\n" << content << "...\n";
}

int main()
{
    auto at = auth().then([](boost::future<ccd::auth_utils::token> t)
    {
        return t.get().access;
    }).share();

    ccd::gdrive::gdrive g { at };
    auto files_rsc = g.files_resource();

    list_root_dir(files_rsc);
    print_getting_started_content(files_rsc);

    return 0;
}
