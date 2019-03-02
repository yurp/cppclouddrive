
#include <ccd/gdrive/resource/gdrive_update.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

update::update(ccd::http::transport_func http_transport,
               std::string file_id,
               std::optional<model::file> metadata_patch,
               std::optional<std::string> media_content)
    : ccd::details::http_executor(std::move(http_transport))
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

ccd::http::request update::build_request()
{
    static const std::string boundary = "ccd_boundary_1234567890987654321";
    static const std::string content_type = "multipart/related; boundary=" + boundary;

    ccd::http::request e;

    std::string media_type = "text/plain";
    std::string patch_content = "{}";
    if (m_metadata_patch)
    {
        media_type = m_metadata_patch->get_mime_type().value_or(media_type);
        patch_content = to_json(m_metadata_patch->to_json());
    }
    e.body = "--" + boundary + "\r\nContent-Type: application/json; charset=UTF-8\r\n\r\n" + patch_content + "\r\n";

    if (m_media_content)
    {
        e.body += "--" + boundary + "\r\n";
        e.body += "Content-Type: " + media_type + "\r\n\r\n";
        e.body += to_json(*m_media_content) + "\r\n";
    }
    e.body += "--" + boundary + "--";
    e.content_type = content_type;

    // TODO: extend to use with other upload types ("media", "resumable")

    e.method = "PATCH";
    e.host = "https://www.googleapis.com";
    e.path = "upload/drive/v3/files/" + m_file_id;
    e.queries.emplace_back("uploadType", "multipart");
    add_change_file_parameters(e);
    add_use_content_as_indexable_text_parameter(e);
    if (m_add_parents)
        e.queries.emplace_back("addParents", *m_add_parents);
    if (m_remove_parents)
        e.queries.emplace_back("removeParents", *m_remove_parents);

    return e;
}

}
}
}
