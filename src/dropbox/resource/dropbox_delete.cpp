
#include <ccd/dropbox/resource/dropbox_delete.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

namespace
{

const auto path_field = "path";
const auto parent_rev_field = "parent_rev";

}

del::del(ccd::http::transport_func http_transport, std::string path)
    : ccd::details::http_executor(std::move(http_transport))
    , m_json(var::map_t{})
{
    m_json[path_field] = std::move(path);
}

ccd::http::request del::build_request()
{
    ccd::http::request e;
    e.method = "POST";
    e.host = "https://api.dropboxapi.com";
    e.path = "/2/files/delete_v2";
    e.content_type = "application/json";
    e.body = to_json(m_json);

    return e;
}

del& del::set_parent_rev(std::string x)
{
    m_json[parent_rev_field] = x;
    return *this;
}

boost::future<model::metadata> del::exec()
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