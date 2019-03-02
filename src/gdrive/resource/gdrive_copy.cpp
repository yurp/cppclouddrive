
#include <ccd/gdrive/resource/gdrive_copy.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

copy::copy(ccd::http::transport_func http_transport, std::string file_id, model::file metadata_patch)
    : ccd::details::http_executor(std::move(http_transport))
    , m_file_id(std::move(file_id))
    , m_metadata_patch(std::move(metadata_patch))
{

}

copy& copy::set_file_id(std::string x)
{
    m_file_id = std::move(x);
    return *this;
}

boost::future<model::file> copy::exec()
{
    return exec_custom<model::file>();
}

ccd::http::request copy::build_request()
{
    ccd::http::request e;
    e.method = "POST";
    e.host = "https://www.googleapis.com";
    e.path = "drive/v3/files/" + m_file_id + "/copy";
    add_change_file_parameters(e);
    add_ignore_default_visibility_parameter(e);
    e.body = to_json(m_metadata_patch.to_json());
    e.content_type = "application/json; charset=utf-8";

    return e;
}

}
}
}
