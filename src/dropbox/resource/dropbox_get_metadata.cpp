
#include <ccd/dropbox/resource/dropbox_get_metadata.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

namespace
{

const auto path_field = "path";
const auto include_media_info_field = "include_media_info";
const auto include_deleted_field = "include_deleted";
const auto include_has_explicit_shared_members_field = "include_has_explicit_shared_members";

}

get_metadata::get_metadata(ccd::http::transport_func http_transport, std::string path)
    : ccd::details::http_executor(std::move(http_transport))
      , m_json(var::map_t{})
{
    m_json[path_field] = std::move(path);
}

get_metadata& get_metadata::set_include_media_info(bool x)
{
    m_json[include_media_info_field] = x;
    return *this;
}

get_metadata& get_metadata::set_include_deleted(bool x)
{
    m_json[include_deleted_field] = x;
    return *this;
}

get_metadata& get_metadata::set_include_has_explicit_shared_members(bool x)
{
    m_json[include_has_explicit_shared_members_field] = x;
    return *this;
}

ccd::http::request get_metadata::build_request()
{
    ccd::http::request e;
    e.method = "POST";
    e.host = "https://api.dropboxapi.com";
    e.path = "/2/files/get_metadata";
    e.content_type = "application/json";
    e.body = to_json(m_json);

    return e;
}

boost::future<model::metadata> get_metadata::exec()
{
    return exec_raw_json().then([](boost::future<var> jsf)
    {
        return model::metadata { jsf.get() };
    });
}

}
}
}
