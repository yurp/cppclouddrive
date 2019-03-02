
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

class update : public ccd::details::http_executor,
               public details::with_change_file_parameters<update>,
               public details::with_use_content_as_indexable_text_parameter<update>
{
    friend class files;

public:
    /// @brief The ID of the file
    update& set_file_id(std::string x);

    /// @brief A comma-separated list of parent IDs to add
    update& set_add_parents(std::optional<std::string> x);

    /// @brief A comma-separated list of parent IDs to remove
    update& set_remove_parents(std::optional<std::string> x);

    boost::future<model::file> exec();

private:
    update(ccd::http::transport_func http_transport,
           std::string file_id,
           std::optional<model::file> metadata_patch,
           std::optional<std::string> media_content);

    ccd::http::request build_request() override;

    std::string m_file_id;
    std::optional<model::file> m_metadata_patch;
    std::optional<std::string> m_media_content;
    std::optional<std::string> m_add_parents;
    std::optional<std::string> m_remove_parents;
};

}
}
}
