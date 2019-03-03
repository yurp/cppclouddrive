
#include <ccd/dropbox/resource/dropbox_download.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

namespace
{

const auto path_field = "path";

}

download::download(ccd::http::transport_func http_transport, std::string path)
    : ccd::details::http_executor(std::move(http_transport))
    , m_json(var::map_t{})
{
    m_json[path_field] = std::move(path);
}

ccd::http::request download::build_request()
{
    ccd::http::request e;
    e.method = "POST";
    e.host = "https://content.dropboxapi.com";
    e.path = "2/files/download";

    e.headers.emplace_back("Dropbox-API-Arg", to_json(m_json));
    if (m_range)
        e.headers.emplace_back("Range", "bytes=" + std::to_string(m_range->first) + "-" + std::to_string(m_range->second));

    return e;
}

download& download::reset_range()
{
    m_range = std::nullopt;
    return *this;
}

download& download::set_range(int64_t offset, int64_t sz)
{
    m_range = std::make_pair(offset, offset + sz - 1);
    return *this;
}

boost::future<std::string> download::exec()
{
    return exec_raw_string();
}

}
}
}
