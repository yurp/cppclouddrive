
#include <ccd/auth.h>
#include <ccd/http/cpprest_transport.h>
#include <ccd/http/beast_transport.h>
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

boost::future<ccd::auth::oauth2::token> auth()
{
    using namespace ccd::auth::oauth2;

    std::string token_file = "token.json";
    std::string redirect_uri = "http://localhost:25000/";

    auto oa2token = load_token(token_file);
    if (!oa2token.refresh.empty())
    {
        return gdrive::refresh(GDRIVE_APP_ID, GDRIVE_SECRET_KEY, oa2token.refresh);
    }

    if (!oa2token.access.empty())
    {
        return boost::make_ready_future(oa2token);
    }

    return gdrive::automatic(GDRIVE_APP_ID, GDRIVE_SECRET_KEY, redirect_uri).then([token_file](boost::future<token> ft)
    {
        auto t = ft.get();
        save_token(t, token_file);
        return t;
    });
}

boost::future<void> list_root_dir(ccd::gdrive::v3::resource::files::files& files_rsc)
{
    return files_rsc.list_request().set_page_size(999)
                                             .set_fields("files(id,name,mimeType,size,createdTime)")
                                             .set_q("'root' in parents")
                                             .exec()
    .then([](boost::future<ccd::gdrive::model::file_list> f)
    {
        if (auto files = f.get().get_files())
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
    });
}

boost::future<void> print_getting_started_content(ccd::gdrive::v3::resource::files::files files_rsc)
{
    return files_rsc.list_request().set_page_size(1)
                                   .set_fields("files(id,mimeType)")
                                   .set_q("'root' in parents and name='Getting started'")
                                   .exec()
    .then([files_rsc = std::move(files_rsc)](boost::future<ccd::gdrive::model::file_list> f) mutable
    {
        auto file_list = f.get();

        auto files = file_list.get_files();
        if (!files || files->empty())
        {
            std::cerr << "responce doesn't contain files\n";
            return boost::make_ready_future();
        }

        const auto& f0 = (*files)[0];
        auto id = f0.get_id();
        if (!id || f0.get_mime_type().value_or("") != "application/pdf")
        {
            std::cerr << "Getting started PDF has not been found\n";
            return boost::make_ready_future();
        }

        return files_rsc.get_request(*id).exec_media(/*offset=*/0, /*size=*/128).then([](boost::future<std::string> c)
        {
            std::cout << "Getting started pdf:\n" << c.get() << "...\n";
        });
    }).unwrap();

}

int main()
{
    auth().then([](boost::future<ccd::auth::oauth2::token> t)
    {
        //ccd::http::authorized_oauth2_transport_factory f { t.get().access, ccd::http::cpprest_transport_factory };
        ccd::http::authorized_oauth2_transport_factory f{ t.get().access, ccd::http::beast_transport_factory };
        ccd::gdrive::gdrive g { std::move(f) };
        auto files_rsc = g.files_resource();

        return when_all(boost::make_ready_future(g.files_resource()), list_root_dir(files_rsc));
    })
    .unwrap().then([](ccd::future_tuple<ccd::gdrive::resource::files::files, void> f)
    {
        auto [ f1, f2 ]  = f.get();
        auto files_rsc = f1.get();
        f2.get();
        return print_getting_started_content(files_rsc);
    })
    .unwrap().then([](boost::future<void> f)
    {
        try
        {
            f.get();
        }
        catch (const ccd::http::exception& e)
        {
            std::cerr << "failed with code: " << e.http_code() << ", message:\n" << e.what() << "\n";
        }

    }).get();

    return 0;
}
