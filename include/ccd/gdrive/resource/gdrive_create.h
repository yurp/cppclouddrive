
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
    boost::future<model::file> exec();

private:
    create(boost::shared_future<std::string> token, model::file metadata, std::optional<std::string> media_content);

    boost::future<executor::executor_ptr> build_request() override;

    boost::shared_future<std::string> m_token;
    model::file m_metadata;
    std::optional<std::string> m_media_content;
};

}
}
}
