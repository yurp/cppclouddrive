
#include <ccd/gdrive/resource/gdrive_get.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

get::get(ccd::http::transport_func http_transport, std::string file_id)
    : ccd::details::http_executor(std::move(http_transport))
    , m_file_id(std::move(file_id))
{

}

get& get::set_file_id(std::string file_id)
{
    m_file_id = std::move(file_id);
    return *this;
}

get& get::set_acknowledge_abuse(std::optional<bool> x)
{
    m_acknowledge_abuse = std::move(x);
    return *this;
}

boost::future<std::string> get::exec_media()
{
    set_alt("media");
    m_range = std::nullopt;
    return exec_raw_string();
}

boost::future<std::string> get::exec_media(int64_t offset, int64_t sz)
{
    set_alt("media");
    m_range = std::make_pair(offset, offset + sz - 1);
    return exec_raw_string();
}

boost::future<model::file> get::exec()
{
    set_alt(std::nullopt);
    m_range = std::nullopt;
    return exec_custom<model::file>();
}

ccd::http::request get::build_request()
{
    ccd::http::request e;
    e.method = "GET";
    e.host = "https://www.googleapis.com";
    e.path = "/drive/v3/files/" + m_file_id;
    add_file_parameters(e);

    if (m_acknowledge_abuse)
        e.queries.emplace_back("acknowledgeAbuse", *m_acknowledge_abuse ? "true" : "false");

    if (m_range)
        e.headers.emplace_back("Range",
                               "bytes=" + std::to_string(m_range->first) + "-" + std::to_string(m_range->second));
    return e;
}

}
}
}
