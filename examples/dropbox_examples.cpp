
#include <ccd/auth.h>
#include <ccd/dropbox/dropbox.h>
#include <ccd/http/cpprest_transport.h>
#include <ccd/http/beast_transport.h>

#include <iostream>

// set your app id here or define outside
#ifndef DROPBOX_APP_ID
#define DROPBOX_APP_ID ""
#endif

// set your app's secret key here or define outside
#ifndef DROPBOX_SECRET_KEY
#define DROPBOX_SECRET_KEY ""
#endif


boost::future<ccd::auth::oauth2::token> auth()
{
    using namespace ccd::auth::oauth2;

    std::string token_file = "token.json";
    std::string redirect_uri = "http://localhost:25000/";

    auto oa2token = load_token(token_file);
    if (!oa2token.access.empty())
    {
        return boost::make_ready_future(oa2token);
    }

    return dropbox::automatic(DROPBOX_APP_ID, DROPBOX_SECRET_KEY, redirect_uri).then([token_file](boost::future<token> ft)
        {
            auto t = ft.get();
            save_token(t, token_file);
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
    auth().then([](boost::future<ccd::auth::oauth2::token> t)
    {
        //ccd::http::authorized_oauth2_transport_factory f{ t.get().access, ccd::http::cpprest_transport_factory };
        ccd::http::authorized_oauth2_transport_factory f { t.get().access, ccd::http::beast_transport_factory };
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

        auto s = d.files_resource().download_request("/main.cpp").exec().get();
        std::cerr << s << "\n";
        s = d.files_resource().download_request("/main.cpp").set_range(5, 10).exec().get();
        std::cerr << s << "\n";

        ccd::dropbox::model::metadata_list_t lst;
        if (auto l = mdlst.get_entries())
        {
            lst = std::move(*l);
        }

        return mdlst.get_has_more().value_or(false) ? list_dir(std::move(lst), d.files_resource(), *mdlst.get_cursor())
                                                    : boost::make_ready_future(lst);
    })
    .unwrap().then([](auto f)
    {
        ccd::dropbox::model::metadata_list_t lst = f.get();
        std::cerr << "-------\n";
        for (auto& i: lst)
        {
            if (auto mi = i.get_media_info())
            {
                mi->set_time_taken("200-01-01T00:00:00Z");
                i.set_media_info(mi);
            }
            std::cerr << ccd::to_json(i.to_json()) << "\n";
            //std::cerr << i.get_path_display().value_or("?")
            //          << "\t[" << i.get_id().value_or("?") << "]"
            //          << ", " << i.get_size().value_or(-1)
            //          << "\n";
        }
    })
    .get();

    return 0;
}
