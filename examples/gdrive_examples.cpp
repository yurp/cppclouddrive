
#include <ccd/auth.h>
#include <ccd/http/beast_transport.h>
#include <ccd/gdrive/gdrive.h>

#include <iostream>

boost::future<ccd::auth::oauth2::token> auth()
{
    std::string token_file = "/Users/iurii/proj/cld/tokens/gdrive.yurii.pelykh.json";
    std::string redirect_uri = "http://localhost:25000/";
    auto app_id = std::getenv("GDRIVE_APP_ID");
    auto app_secret = std::getenv("GDRIVE_SECRET_KEY");
    if (app_id == nullptr || app_secret == nullptr)
    {
        boost::throw_exception(std::runtime_error{ "app credentials aren't set" });
    }

    ccd::auth::oauth2_gdrive oa2 { app_id, app_secret };
    auto oa2token = ccd::auth::oauth2::load_token(token_file);
    if (!oa2token.refresh.empty())
    {
        return oa2.refresh(oa2token.refresh);
    }

    if (!oa2token.access.empty())
    {
        return boost::make_ready_future(oa2token);
    }

    return oa2.automatic(redirect_uri).then([token_file](boost::future<ccd::auth::oauth2::token> ft)
    {
        auto t = ft.get();
        ccd::auth::oauth2::save_token(t, token_file);
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
    boost::asio::io_service ios;
    boost::asio::io_service::work w { ios };

    auto f = auth().then([&ios](boost::future<ccd::auth::oauth2::token> t)
    {
        ccd::http::authorized_oauth2_transport_factory f{ t.get().access, ccd::http::async_beast_transport_factory(ios) };
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

    })
    .then([&ios](boost::future<void> )
    {
        ios.stop();
    });

    ios.run();
    f.get();

    return 0;
}
