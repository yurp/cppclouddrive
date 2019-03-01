
#include <ccd/gdrive/resource/gdrive_update.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

update::update(boost::shared_future<std::string> token,
               std::string file_id,
               std::optional<model::file> metadata_patch,
               std::optional<std::string> media_content)
    : m_token(std::move(token))
    , m_file_id(std::move(file_id))
    , m_metadata_patch(std::move(metadata_patch))
    , m_media_content(std::move(media_content))
{

}

update& update::set_file_id(std::string x)
{
    m_file_id = std::move(x);
    return *this;
}

update& update::set_add_parents(std::optional<std::string> x)
{
    m_add_parents = std::move(x);
    return *this;
}

update& update::set_remove_parents(std::optional<std::string> x)
{
    m_remove_parents = std::move(x);
    return *this;
}

boost::future<model::file> update::exec()
{
    return exec_custom<model::file>();
}

boost::future<executor::executor_ptr> update::build_request()
{
    static const std::string boundary = "ccd_boundary_1234567890987654321";
    static const std::string content_type = "multipart/related; boundary=" + boundary;

    std::string media_type = "text/plain";
    std::string patch_content = "{}";
    if (m_metadata_patch)
    {
        media_type = m_metadata_patch->get_mime_type().value_or(media_type);
        patch_content = to_json(m_metadata_patch->to_json());
    }

    std::string body = "--" + boundary + "\r\n"
                       "Content-Type: application/json; charset=UTF-8\r\n\r\n" +
                       patch_content + "\r\n";

    if (m_media_content)
    {
        body += "--" + boundary + "\r\n";
        body += "Content-Type: " + media_type + "\r\n\r\n";
        body += to_json(*m_media_content) + "\r\n";
    }
    body += "--" + boundary + "--";

    std::cerr << "body: " << body << "\n";

    // TODO: extend to use with other upload types ("media", "resumable")

    std::shared_ptr<http_executor> e = std::make_shared<cpprest_executor>();
    e->set_method("PATCH");
    e->set_endpoint("https://www.googleapis.com");
    e->append_path("upload/drive/v3/files");
    e->append_path(m_file_id);
    e->append_query("uploadType", "multipart");
    add_change_file_parameters(*e);
    add_use_content_as_indexable_text_parameter(*e);
    if (m_add_parents)
        e->append_query("addParents", *m_add_parents);
    if (m_remove_parents)
        e->append_query("removeParents", *m_remove_parents);
    e->set_body(body, content_type);

    return m_token.then([e = std::move(e)](boost::shared_future<std::string> t)
    {
        e->set_oauth2_token(t.get());
        return e;
    });
}

}
}
}
