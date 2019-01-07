
#include <ccd/gdrive/resource/gdrive_copy.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

copy::copy(pplx::task<http_client_ptr> client, std::string file_id, model::file metadata_patch)
    : executor(std::move(client))
    , m_file_id(std::move(file_id))
    , m_metadata_patch(std::move(metadata_patch))
{

}

copy& copy::set_file_id(std::string x)
{
    m_file_id = std::move(x);
    return *this;
}

pplx::task<model::file> copy::exec()
{
    return exec_custom<model::file>();
}

web::http::http_request copy::build_request()
{
    auto ub = web::uri_builder{ }.set_path("drive/v3/files").append_path(m_file_id).append_path("copy");
    add_change_file_parameters(ub);
    add_ignore_default_visibility_parameter(ub);

    web::http::http_request r;
    r.set_method(web::http::methods::POST);
    r.set_body(utility::conversions::to_utf8string(m_metadata_patch.to_json().serialize()),
               "application/json; charset=utf-8");
    r.set_request_uri(ub.to_uri());

    return r;
}

}
}
}
