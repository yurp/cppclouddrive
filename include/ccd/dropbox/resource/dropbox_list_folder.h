
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/executor.h>
#include <ccd/dropbox/model/dropbox_metadata.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

class list_folder : public executor
{
public:
    list_folder(pplx::task<http_client_ptr> client, std::string path = "");

    list_folder& set_recursive(bool x);
    list_folder& set_include_media_info(bool x);
    list_folder& set_include_deleted(bool x);
    list_folder& set_include_has_explicit_shared_members(bool x);
    list_folder& set_include_mounted_folders(bool x);
    list_folder& set_limit(int32_t x);
    list_folder& set_shared_link(std::string url, std::optional<std::string> password = std::nullopt);
    // TODO: add include_property_groups

    pplx::task <model::metadata_list> exec();

private:
    web::http::http_request build_request() override;
    web::json::value m_json;
};

class list_folder_continue : public executor
{
public:
    list_folder_continue(pplx::task<http_client_ptr> client, std::string cursor);
    pplx::task <model::metadata_list> exec();

private:
    web::http::http_request build_request() override;
    web::json::value m_json;
};

}
}
}
