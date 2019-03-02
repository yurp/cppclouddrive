
#include <ccd/gdrive/resource/gdrive_create.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

create::create(ccd::http::transport_func http_transport, model::file metadata, std::optional<std::string> media_content)
    : ccd::details::http_executor(std::move(http_transport))
    , m_metadata(std::move(metadata))
    , m_media_content(std::move(media_content))
{

}

boost::future<model::file> create::exec()
{
    return exec_custom<model::file>();
}

// see content types here: https://www.iana.org/assignments/media-types/media-types.xhtml
ccd::http::request create::build_request()
{
    static const std::string boundary = "ccd_boundary_1234567890987654321";
    static const std::string content_type = "multipart/related; boundary=" + boundary;

    ccd::http::request e;
    e.body = "--" + boundary + "\r\n"
                    "Content-Type: application/json; charset=UTF-8\r\n\r\n" +
                    to_json(m_metadata.to_json()) + "\r\n";

    if (m_media_content)
    {
        e.body += "--" + boundary + "\r\n";
        e.body += "Content-Type: " + m_metadata.get_mime_type().value_or("text/plain") + "\r\n\r\n";
        e.body += *m_media_content + "\r\n";
    }
    e.body += "--" + boundary + "--";
    e.content_type = content_type;

    // TODO: extend to use with other upload types ("media", "resumable")

    e.method = "POST";
    e.host = "https://www.googleapis.com";
    e.path = "upload/drive/v3/files";
    e.queries.emplace_back("uploadType", "multipart");
    add_change_file_parameters(e);
    add_ignore_default_visibility_parameter(e);
    add_use_content_as_indexable_text_parameter(e);

    return e;
}

}
}
}
