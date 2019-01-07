
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/executor.h>
#include <ccd/gdrive/details/gdrive_parameters.h>
#include <ccd/gdrive/model/gdrive_file.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

class files;

class copy : public executor,
             public details::with_change_file_parameters<copy>,
             public details::with_ignore_default_visibility_parameter<copy>
{
    friend class files;

public:
    /// @brief The ID of the file
    copy& set_file_id(std::string x);

    pplx::task <model::file> exec();

private:
    copy(pplx::task <http_client_ptr> client, std::string file_id, model::file metadata_patch);

    web::http::http_request build_request() override;

    std::string m_file_id;
    model::file m_metadata_patch;
};

}
}
}
