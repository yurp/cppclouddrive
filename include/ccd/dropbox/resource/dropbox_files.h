
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/dropbox/resource/dropbox_list_folder.h>

namespace ccd::dropbox
{
inline namespace v2
{

class dropbox;

namespace resource::files
{

class files
{
    friend class ccd::dropbox::v2::dropbox;

public:
    list_folder list_folder_request(std::string path = "");
    list_folder_continue list_folder_continue_request(std::string cursor);

private:
    files(pplx::task<web::http::client::http_client_config> client_config);

    pplx::task<web::http::client::http_client_config> m_client_config;
};

}
}
}
