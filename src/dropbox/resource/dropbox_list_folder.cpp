
#include <ccd/dropbox/resource/dropbox_list_folder.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

namespace
{

const auto path_field = "path";
const auto recursive_field = "recursive";
const auto include_media_info_field = "include_media_info";
const auto include_deleted_field = "include_deleted";
const auto include_has_explicit_shared_members_field = "include_has_explicit_shared_members";
const auto include_mounted_folders_field = "include_mounted_folders";
const auto shared_link_field = "shared_link";
const auto limit_field = "limit";
const auto url_field = "url";
const auto password_field = "password";

const auto cursor_field = "cursor"; // list_folder_continue

}

list_folder::list_folder(ccd::http::transport_func http_transport, std::string path)
    : ccd::details::http_executor(std::move(http_transport))
    , m_json(var::map_t{})
{
    m_json[path_field] = std::move(path);
}

list_folder& list_folder::set_recursive(bool x)
{
    m_json[recursive_field] = x;
    return *this;
}

list_folder& list_folder::set_include_media_info(bool x)
{
    m_json[include_media_info_field] = x;
    return *this;
}

list_folder& list_folder::set_include_deleted(bool x)
{
    m_json[include_deleted_field] = x;
    return *this;
}

list_folder& list_folder::set_include_has_explicit_shared_members(bool x)
{
    m_json[include_has_explicit_shared_members_field] = x;
    return *this;
}

list_folder& list_folder::set_include_mounted_folders(bool x)
{
    m_json[include_mounted_folders_field] =  x;
    return *this;
}

list_folder& list_folder::set_limit(int32_t x)
{
    m_json[limit_field] = x;
    return *this;
}

list_folder& list_folder::set_shared_link(std::string url, std::optional<std::string> password)
{
    m_json[shared_link_field] = var::map_t{};
    m_json[shared_link_field][url_field] = std::move(url);
    if (password)
    {
        m_json[shared_link_field][password_field] = *password;
    }
    return *this;
}

ccd::http::request list_folder::build_request()
{
    ccd::http::request e;
    e.method = "POST";
    e.host = "https://api.dropboxapi.com";
    e.path = "/2/files/list_folder";
    e.content_type = "application/json";
    e.body = to_json(m_json);

    return e;
}

boost::future<model::metadata_list> list_folder::exec()
{
    return exec_custom<model::metadata_list>();
}

list_folder_continue::list_folder_continue(ccd::http::transport_func http_transport, std::string cursor)
    : ccd::details::http_executor(std::move(http_transport))
    , m_json(var::map_t{})
{
    m_json[cursor_field] = std::move(cursor);
}

boost::future<model::metadata_list> list_folder_continue::exec()
{
    return exec_custom<model::metadata_list>();
}

ccd::http::request list_folder_continue::build_request()
{
    ccd::http::request e;
    e.method = "POST";
    e.host = "https://api.dropboxapi.com";
    e.path = "/2/files/list_folder/continue";
    e.content_type = "application/json";
    e.body = to_json(m_json);

    return e;
}

}
}
}
