
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

class list_folder : public ccd::details::http_executor
{
    friend class files;

public:
    list_folder& set_recursive(bool x);
    list_folder& set_include_media_info(bool x);
    list_folder& set_include_deleted(bool x);
    list_folder& set_include_has_explicit_shared_members(bool x);
    list_folder& set_include_mounted_folders(bool x);
    list_folder& set_limit(int32_t x);
    list_folder& set_shared_link(std::string url, std::optional<std::string> password = std::nullopt);
    // TODO: add include_property_groups

    boost::future<model::metadata_list> exec();

private:
    list_folder(ccd::http::transport_func http_transport, std::string path);
    ccd::http::request build_request() override;

    var m_json;
};

class list_folder_continue : public ccd::details::http_executor
{
    friend class files;

public:
    boost::future<model::metadata_list> exec();

private:
    list_folder_continue(ccd::http::transport_func http_transport, std::string cursor);
    ccd::http::request build_request() override;

    var m_json;
};

}
}
}
