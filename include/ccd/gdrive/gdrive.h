
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
    explicit gdrive(boost::shared_future<std::string> token);

    resource::files::files files_resource();

private:
    boost::shared_future<std::string> m_token;
};

}
}
