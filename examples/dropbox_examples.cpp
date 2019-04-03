
#include <ccd/auth.h>
#include <ccd/dropbox/dropbox.h>
#include <ccd/http/beast_transport.h>

#include <iostream>

boost::future<ccd::auth::oauth2::token> auth()
{
    std::string token_file = "/Users/iurii/proj/cld/tokens/dropbox.yurp1980.json";
    std::string redirect_uri = "http://localhost:25000/";
    auto app_id = std::getenv("DROPBOX_APP_ID");
    auto app_secret = std::getenv("DROPBOX_SECRET_KEY");
    if (app_id == nullptr || app_secret == nullptr)
    {
        boost::throw_exception(std::runtime_error{ "app credentials aren't set" });
    }

    auto oa2token = ccd::auth::oauth2::load_token(token_file);
    if (!oa2token.access.empty())
    {
        return boost::make_ready_future(oa2token);
    }

    ccd::auth::oauth2_dropbox oa2 { app_id, app_secret };
    return oa2.automatic(redirect_uri).then([token_file](boost::future<ccd::auth::oauth2::token> ft)
        {
            auto t = ft.get();
            ccd::auth::oauth2::save_token(t, token_file);
            return t;
        });
}

boost::future<ccd::dropbox::model::metadata_list_t> list_dir(ccd::dropbox::model::metadata_list_t lst,
                                                             ccd::dropbox::resource::files::files file_res,
                                                             std::string cursor)
{
    return file_res.list_folder_continue_request(cursor).exec()
        .then([lst = std::move(lst), file_res = std::move(file_res)](auto ft) mutable
        {
            ccd::dropbox::model::metadata_list mdlst = ft.get();
            if (auto l = mdlst.get_entries())
            {
                lst.insert(end(lst), begin(*l), end(*l));
            }

            return mdlst.get_has_more().value_or(false)
                ? list_dir(std::move(lst), std::move(file_res), *mdlst.get_cursor())
                : boost::make_ready_future(lst);
        })
        .unwrap();
}


int main()
{
    boost::asio::io_service ios;
    boost::asio::io_service::work w { ios };

    auto f = auth().then([&ios](boost::future<ccd::auth::oauth2::token> t)
    {
        ccd::http::authorized_oauth2_transport_factory f{ t.get().access, ccd::http::async_beast_transport_factory(ios) };
        ccd::dropbox::dropbox d{ std::move(f) };
        auto file_res = d.files_resource();

        return when_all(boost::make_ready_future(std::move(d)),
                        file_res.download_request("/main.cpp").exec(),
                        file_res.download_request("/main.cpp").set_range(5, 10).exec(),
                        file_res.list_folder_request("/tmp").set_recursive(true)
                                                            .set_include_deleted(true)
                                                            .set_include_media_info(true)
                                                            .set_limit(999)
                                                            .exec());
    })
    .unwrap().then([](ccd::future_tuple<ccd::dropbox::dropbox,
                                        std::string,
                                        std::string,
                                        ccd::dropbox::model::metadata_list> f)
    {
        auto [ f1, f2, f3, f4 ] = f.get();
        ccd::dropbox::dropbox d = f1.get();
        auto s1 = f2.get();
        auto s2 = f3.get();
        auto mdlst = f4.get();

        std::cout << s1 << "\n";
        std::cout << s2 << "\n";

        ccd::dropbox::model::metadata_list_t lst;
        if (auto l = mdlst.get_entries())
        {
            lst = std::move(*l);
        }

        auto list_future = mdlst.get_has_more().value_or(false)
            ? list_dir(std::move(lst), d.files_resource(), *mdlst.get_cursor())
            : boost::make_ready_future(lst);
        return when_all(boost::make_ready_future(std::move(d)), std::move(list_future));
    })
    .unwrap().then([](ccd::future_tuple<ccd::dropbox::dropbox, ccd::dropbox::model::metadata_list_t> f)
    {
        auto [ f1, f2 ] = f.get();
        ccd::dropbox::model::metadata_list_t lst = f2.get();
        std::cout << "-------\n";
        for (auto& i: lst)
        {
            if (auto mi = i.get_media_info())
            {
                mi->set_time_taken("200-01-01T00:00:00Z");
                i.set_media_info(mi);
            }
            std::cout << ccd::to_json(i.to_json()) << "\n";
            //std::cerr << i.get_path_display().value_or("?")
            //          << "\t[" << i.get_id().value_or("?") << "]"
            //          << ", " << i.get_size().value_or(-1)
            //          << "\n";
        }

        return f1.get();
    })
    .then([](boost::future<ccd::dropbox::dropbox> f)
    {
        auto files_rsc = f.get().files_resource();
        return files_rsc.copy_request("/main.cpp", "/main2.cpp").set_autorename(true).exec();
    })
    .unwrap().then([](boost::future<ccd::dropbox::model::metadata> f)
    {
        std::cout << ccd::to_json(f.get().to_json()) << "\n";
        //return files_rsc.
    })
    .then([](boost::future<void> f)
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
