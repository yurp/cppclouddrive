
// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/gdrive/model/gdrive_file.h>

#include <boost/asio.hpp>

#include <gmock/gmock.h>

#include <iostream>
#include <ccd/gdrive/gdrive.h>

TEST(gdrive_copy, test)
{
    ccd::http::authorized_oauth2_transport_factory factory { "access_token_1234567890", []
    {
        return [](ccd::http::request r)
        {
            EXPECT_EQ(r.method, "POST");

            return boost::make_ready_future<ccd::http::response>();
        };
    } };

    ccd::var v;

    ccd::gdrive::gdrive g(factory);
    //g.files_resource().copy_request("file_id_1234567899",)

}
