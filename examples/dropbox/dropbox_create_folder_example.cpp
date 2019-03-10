
#include "dropbox_auth_example.h"

#include <ccd/dropbox/dropbox.h>
#include <ccd/http/beast_transport.h>

#include <cstdlib>
#include <iostream>

int main()
{
    return auth().then([](boost::future<ccd::auth::oauth2::token> t)
    {
        ccd::http::authorized_oauth2_transport_factory f { t.get().access, ccd::http::beast_transport_factory };
        ccd::dropbox::dropbox d{ std::move(f) };
        return d.files_resource().create_folder_request("/cpp_folder").set_autorename(true).exec();

    })
    .unwrap().then([](boost::future<ccd::dropbox::model::metadata> f)
    {
        try
        {
            auto md = f.get();
            std::cout << "created folder '" << md.get_name().value_or("?")
                      << "', path: " << md.get_path_display().value_or("?");
            return 0;
        }
        catch (const ccd::http::exception& e)
        {
            std::cout << "error: http code " << e.http_code() << ", message:\n" << e.what() << "\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "error: " << e.what() << "\n";
        }

        return -1;
    })
    .get();
}
