
#include <ccd/gdrive/resource/gdrive_create.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

create::create(boost::shared_future<std::string> token, model::file metadata, std::optional<std::string> media_content)
    : m_token(std::move(token))
    , m_metadata(std::move(metadata))
    , m_media_content(std::move(media_content))
{

}

boost::future<model::file> create::exec()
{
    return exec_custom<model::file>();
}

// see content types here: https://www.iana.org/assignments/media-types/media-types.xhtml
boost::future<executor::executor_ptr> create::build_request()
{
    static const std::string boundary = "ccd_boundary_1234567890987654321";
    static const std::string content_type = "multipart/related; boundary=" + boundary;

    std::string body = "--" + boundary + "\r\n"
                       "Content-Type: application/json; charset=UTF-8\r\n\r\n" +
                       to_json(m_metadata.to_json()) + "\r\n";

    if (m_media_content)
    {
        body += "--" + boundary + "\r\n";
        body += "Content-Type: " + m_metadata.get_mime_type().value_or("text/plain") + "\r\n\r\n";
        body += *m_media_content + "\r\n";
    }
    body += "--" + boundary + "--";

    // TODO: extend to use with other upload types ("media", "resumable")

    std::shared_ptr<http_executor> e = std::make_shared<cpprest_executor>();
    e->set_method("POST");
    e->set_endpoint("https://www.googleapis.com");
    e->append_path("upload/drive/v3/files");
    e->append_query("uploadType", "multipart");
    add_change_file_parameters(*e);
    add_ignore_default_visibility_parameter(*e);
    add_use_content_as_indexable_text_parameter(*e);
    e->set_body(content_type, body);

    return m_token.then([e = std::move(e)](boost::shared_future<std::string> t)
    {
        e->set_oauth2_token(t.get());
        return e;
    });
}

}
}
}
