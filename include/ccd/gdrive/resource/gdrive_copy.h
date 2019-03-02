
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/details/http_executor.h>
#include <ccd/gdrive/details/gdrive_parameters.h>
#include <ccd/gdrive/model/gdrive_file.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

class files;

class copy : public ccd::details::http_executor,
             public details::with_change_file_parameters<copy>,
             public details::with_ignore_default_visibility_parameter<copy>
{
    friend class files;

public:
    /// @brief The ID of the file
    copy& set_file_id(std::string x);

    boost::future<model::file> exec();

private:
    copy(ccd::http::transport_func http_transport, std::string file_id, model::file metadata_patch);

    ccd::http::request build_request() override;

    boost::shared_future<std::string> m_token;
    std::string m_file_id;
    model::file m_metadata_patch;
};

}
}
}
