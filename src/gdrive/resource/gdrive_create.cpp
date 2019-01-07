
#include <ccd/gdrive/resource/gdrive_create.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

create::create(pplx::task<http_client_ptr> client, model::file metadata, std::optional<std::string> media_content)
    : executor(std::move(client))
    , m_metadata(std::move(metadata))
    , m_media_content(std::move(media_content))
{

}

pplx::task<model::file> create::exec()
{
    return exec_custom<model::file>();
}

// see content types here: https://www.iana.org/assignments/media-types/media-types.xhtml
web::http::http_request create::build_request()
{
    static const std::string boundary = "ccd_boundary_1234567890987654321";
    static const std::string content_type = "multipart/related; boundary=" + boundary;

    std::string body = "--" + boundary + "\r\n"
                                         "Content-Type: application/json; charset=UTF-8\r\n\r\n" +
                       utility::conversions::to_utf8string(m_metadata.to_json().serialize()) + "\r\n";

    if (m_media_content)
    {
        body += "--" + boundary + "\r\n";
        body += "Content-Type: " + m_metadata.get_mime_type().value_or("text/plain") + "\r\n\r\n";
        body += utility::conversions::to_utf8string(*m_media_content) + "\r\n";
    }
    body += "--" + boundary + "--";

    // TODO: extend to use with other upload types ("media", "resumable")
    auto ub = web::uri_builder{ }.set_path("upload/drive/v3/files").append_query("uploadType", "multipart");
    add_change_file_parameters(ub);
    add_ignore_default_visibility_parameter(ub);
    add_use_content_as_indexable_text_parameter(ub);

    web::http::http_request r;
    r.set_method(web::http::methods::POST);
    r.set_body(body, content_type);
    r.set_request_uri(ub.to_uri());

    return r;
}

}
}
}
