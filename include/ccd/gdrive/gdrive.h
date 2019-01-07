
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/gdrive/resource/gdrive_files.h>

namespace ccd::gdrive
{
inline namespace v3
{

class gdrive
{
public:
    explicit gdrive(pplx::task<http_client_ptr> client);

    resource::files::files files_resource();

private:
    pplx::task<http_client_ptr> m_client;
};

}
}
