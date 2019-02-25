
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/dropbox/resource/dropbox_files.h>

namespace ccd::dropbox
{
inline namespace v2
{

class dropbox
{
public:
    explicit dropbox(pplx::task<web::http::client::http_client_config> client_config);

    resource::files::files files_resource();

private:
    pplx::task<web::http::client::http_client_config> m_client_config;
};

}
}
