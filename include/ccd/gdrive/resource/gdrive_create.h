
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

class create : public executor,
               public details::with_change_file_parameters<create>,
               public details::with_ignore_default_visibility_parameter<create>,
               public details::with_use_content_as_indexable_text_parameter<create>
{
    friend class files;

public:
    pplx::task<model::file> exec();

private:
    create(pplx::task<http_client_ptr> client, model::file metadata, std::optional<std::string> media_content);

    web::http::http_request build_request() override;

    model::file m_metadata;
    std::optional<std::string> m_media_content;
};

}
}
}
