
#include <ccd/dropbox/resource/dropbox_upload.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

namespace
{

constexpr auto path_field = "path";
constexpr auto mode_field = "mode";
constexpr auto autorename_field = "autorename";
constexpr auto client_modified_field = "client_modified";
constexpr auto mute_field = "mute";
constexpr auto strict_conflict_field = "strict_conflict";

}

upload::upload(ccd::http::transport_func http_transport, std::string path, std::string content)
    : ccd::details::http_executor(std::move(http_transport))
    , m_json(var::map_t{})
    , m_content(std::move(content))
{
    m_json[path_field] = std::move(path);
}

upload& upload::set_add_mode()
{
    m_json[mode_field] = var::map_t { { ".tag", "add" } };
    return *this;
}

upload& upload::set_overwrite_mode()
{
    m_json[mode_field] = var::map_t { { ".tag", "overwrite" } };
    return *this;
}

upload& upload::set_update_mode(std::string x)
{
    m_json[mode_field] = var::map_t { { ".tag", "update" }, { "update", std::move(x) } };
    return *this;
}

upload& upload::set_autorename(bool x)
{
    m_json[autorename_field] = x;
    return *this;
}

upload& upload::set_client_modified(std::string x)
{
    m_json[client_modified_field] = std::move(x);
    return *this;
}

upload& upload::set_mute(bool x)
{
    m_json[mute_field] = x;
    return *this;
}

upload& upload::set_strict_conflict(bool x)
{
    m_json[strict_conflict_field] = x;
    return *this;
}

ccd::http::request upload::build_request()
{
    ccd::http::request e;
    e.method = "POST";
    e.host = "https://content.dropboxapi.com";
    e.path = "/2/files/upload";
    e.content_type = "application/octet-stream";

    e.headers.emplace_back("Dropbox-API-Arg", to_json(m_json));
    e.body = m_content;

    return e;
}

boost::future<model::metadata> upload::exec()
{
    return exec_raw_json().then([](boost::future<var> jsf)
    {
        return model::metadata { jsf.get() };
    });
}

}
}
}
