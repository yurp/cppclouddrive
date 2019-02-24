
#include <ccd/dropbox/resource/dropbox_list_folder.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

using namespace utility::conversions;

namespace
{

const auto path_field = to_string_t("path");
const auto recursive_field = to_string_t("recursive");
const auto include_media_info_field = to_string_t("include_media_info");
const auto include_deleted_field = to_string_t("include_deleted");
const auto include_has_explicit_shared_members_field = to_string_t("include_has_explicit_shared_members");
const auto include_mounted_folders_field = to_string_t("include_mounted_folders");
const auto shared_link_field = to_string_t("shared_link");
const auto limit_field = to_string_t("limit");
const auto url_field = to_string_t("url");
const auto password_field = to_string_t("password");

const auto cursor_field = to_string_t("cursor"); // list_folder_continue

}

list_folder::list_folder(pplx::task<http_client_ptr> client, std::string path)
    : executor(std::move(client))
    , m_json(web::json::value::object())
{
    m_json[path_field] = web::json::value{ to_string_t(std::move(path)) };
}

list_folder& list_folder::set_recursive(bool x)
{
    m_json[recursive_field] = web::json::value{ x };
    return *this;
}

list_folder& list_folder::set_include_media_info(bool x)
{
    m_json[include_media_info_field] = web::json::value{ x };
    return *this;
}

list_folder& list_folder::set_include_deleted(bool x)
{
    m_json[include_deleted_field] = web::json::value{ x };
    return *this;
}

list_folder& list_folder::set_include_has_explicit_shared_members(bool x)
{
    m_json[include_has_explicit_shared_members_field] = web::json::value{ x };
    return *this;
}

list_folder& list_folder::set_include_mounted_folders(bool x)
{
    m_json[include_mounted_folders_field] = web::json::value{ x };
    return *this;
}

list_folder& list_folder::set_limit(int32_t x)
{
    m_json[limit_field] = web::json::value{ x };
    return *this;
}

list_folder& list_folder::set_shared_link(std::string url, std::optional<std::string> password)
{
    m_json[shared_link_field] = web::json::value::object();
    m_json[shared_link_field][url_field] = web::json::value{ to_string_t(std::move(url)) };
    if (password)
    {
        m_json[shared_link_field][password_field] = web::json::value{ to_string_t(*password) };
    }
    return *this;
}

web::http::http_request list_folder::build_request()
{
    web::http::http_request r { web::http::methods::POST };
    r.set_body(m_json);
    r.set_request_uri(utility::conversions::to_string_t("/2/files/list_folder"));

    return r;
}

pplx::task<model::metadata_list> list_folder::exec()
{
    return exec_custom<model::metadata_list>();
}

list_folder_continue::list_folder_continue(pplx::task<http_client_ptr> client, std::string cursor)
    : executor(std::move(client))
    , m_json(web::json::value::object())
{
    m_json[cursor_field] = web::json::value{ to_string_t(std::move(cursor)) };
}

pplx::task<model::metadata_list> list_folder_continue::exec()
{
    return exec_custom<model::metadata_list>();
}


web::http::http_request list_folder_continue::build_request()
{
    web::http::http_request r { web::http::methods::POST };
    r.set_body(m_json);
    r.set_request_uri(utility::conversions::to_string_t("/2/files/list_folder/continue"));

    return r;
}

}
}
}
