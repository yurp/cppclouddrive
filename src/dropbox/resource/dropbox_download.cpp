
#include <ccd/dropbox/resource/dropbox_download.h>

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

}

download::download(pplx::task<http_client_ptr> client, std::string path)
    : executor(std::move(client))
    , m_json(web::json::value::object())
{
    m_json[path_field] = web::json::value{ to_string_t(std::move(path)) };
}

web::http::http_request download::build_request()
{
    web::http::http_request r { web::http::methods::POST };
    r.headers().add("Dropbox-API-Arg", m_json);
    if (m_range)
        r.headers().add("Range", "bytes=" + std::to_string(m_range->first) + "-" + std::to_string(m_range->second));

    r.set_request_uri(utility::conversions::to_string_t("/2/files/download"));

    return r;
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

pplx::task<std::string> download::exec()
{
    return exec_raw_string();
}

}
}
}
