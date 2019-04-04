
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/details/http_executor.h>
#include <ccd/dropbox/model/dropbox_metadata.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

class files;

class get_metadata : public ccd::details::http_executor
{
    friend class files;

public:
    /// @brief If true, FileMetadata.media_info is set for photo and video. The default for this field is False.
    get_metadata& set_include_media_info(bool x);

    /// @brief If true, DeletedMetadata will be returned for deleted file or folder, otherwise LookupError.not_found
    ///        will be returned. The default for this field is False.
    get_metadata& set_include_deleted(bool x);

    /// @brief If true, the results will include a flag for each file indicating whether or not that file has any
    ///        explicit members. The default for this field is False.
    get_metadata& set_include_has_explicit_shared_members(bool x);

    // TODO: add include_property_groups

    boost::future<model::metadata> exec();

private:
    /// @param[in] path (pattern="(/(.|[\r\n])*)|(ns:[0-9]+(/.*)?)") Path in the user's Dropbox to create.
    get_metadata(ccd::http::transport_func http_transport, std::string path);

    ccd::http::request build_request() override;

    var m_json;
};

}
}
}
