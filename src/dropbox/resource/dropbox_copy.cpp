
#include <ccd/dropbox/resource/dropbox_copy.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

namespace
{

const auto from_path_field = "from_path";
const auto to_path_field = "to_path";
const auto allow_shared_folder_field = "allow_shared_folder";
const auto autorename_field = "autorename";
const auto allow_ownership_transfer_field = "allow_ownership_transfer";

}

copy::copy(ccd::http::transport_func http_transport, std::string from_path, std::string to_path)
    : ccd::details::http_executor(std::move(http_transport))
    , m_json(var::map_t{})
{
    m_json[from_path_field] = std::move(from_path);
    m_json[to_path_field] = std::move(to_path);
}

ccd::http::request copy::build_request()
{
    ccd::http::request e;
    e.method = "POST";
    e.host = "https://api.dropboxapi.com";
    e.path = "/2/files/copy_v2";
    e.content_type = "application/json";
    e.body = to_json(m_json);

    return e;
}

copy& copy::set_allow_shared_folder(bool x)
{
    m_json[allow_shared_folder_field] = x;
    return *this;
}

copy& copy::set_autorename(bool x)
{
    m_json[autorename_field] = x;
    return *this;
}

copy& copy::set_allow_ownership_transfer(bool x)
{
    m_json[allow_ownership_transfer_field] = x;
    return *this;
}

boost::future<model::metadata> copy::exec()
{
    return exec_raw_json().then([](boost::future<var> jsf)
    {
        auto js = jsf.get();
        if (js.is<var::map_t>() && js.has("metadata"))
        {
            return model::metadata { std::move(js["metadata"]) };
        }

        return model::metadata {}; // TODO: throw error?
    });
}

}
}
}
