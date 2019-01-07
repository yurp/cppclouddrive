
#include <ccd/gdrive/resource/gdrive_get.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

get::get(pplx::task<http_client_ptr> client, std::string file_id)
    : executor(std::move(client))
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

pplx::task<std::string> get::exec_media()
{
    set_alt("media");
    m_range = std::nullopt;
    return exec_raw_string();
}

pplx::task<std::string> get::exec_media(int64_t offset, int64_t sz)
{
    set_alt("media");
    m_range = std::make_pair(offset, offset + sz - 1);
    return exec_raw_string();
}

pplx::task<model::file> get::exec()
{
    set_alt(std::nullopt);
    m_range = std::nullopt;
    return exec_custom<model::file>();
}

web::http::http_request get::build_request()
{
    auto ub = web::uri_builder{ }.set_path("/drive/v3/files").append_path(m_file_id);
    add_file_parameters(ub);

    if (m_acknowledge_abuse)
        ub.append_query("acknowledgeAbuse", *m_acknowledge_abuse ? "true" : "false");

    web::http::http_request r;
    r.set_request_uri(ub.to_uri());
    if (m_range)
        r.headers().add("Range", "bytes=" + std::to_string(m_range->first) + "-" + std::to_string(m_range->second));

    return r;
}

}
}
}
