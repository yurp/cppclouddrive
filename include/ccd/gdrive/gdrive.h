
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/gdrive/resource/gdrive_files.h>
#include <ccd/http/authorized_oauth2_transport.h>

namespace ccd::gdrive
{
inline namespace v3
{

class gdrive
{
public:
    explicit gdrive(ccd::http::authorized_oauth2_transport_factory http_transport_factory);

    resource::files::files files_resource();

private:
    ccd::http::transport_factory m_http_transport_factory;
};

}
}
