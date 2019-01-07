
#include <ccd/gdrive/resource/gdrive_update.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

update::update(pplx::task<http_client_ptr> client,
               std::string file_id,
               std::optional<model::file> metadata_patch,
               std::optional<std::string> media_content)
    : executor(std::move(client))
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

pplx::task<model::file> update::exec()
{
    return exec_custom<model::file>();
}

web::http::http_request update::build_request()
{
    static const std::string boundary = "ccd_boundary_1234567890987654321";
    static const std::string content_type = "multipart/related; boundary=" + boundary;

    std::string media_type = "text/plain";
    std::string patch_content = "{}";
    if (m_metadata_patch)
    {
        media_type = m_metadata_patch->get_mime_type().value_or(media_type);
        patch_content = utility::conversions::to_utf8string(m_metadata_patch->to_json().serialize());
    }

    std::string body = "--" + boundary + "\r\n"
                                         "Content-Type: application/json; charset=UTF-8\r\n\r\n" +
                       patch_content + "\r\n";

    if (m_media_content)
    {
        body += "--" + boundary + "\r\n";
        body += "Content-Type: " + media_type + "\r\n\r\n";
        body += utility::conversions::to_utf8string(*m_media_content) + "\r\n";
    }
    body += "--" + boundary + "--";

    std::cerr << "body: " << body << "\n";

    // TODO: extend to use with other upload types ("media", "resumable")
    auto ub = web::uri_builder{ }.set_path("upload/drive/v3/files")
        .append_path(m_file_id)
        .append_query("uploadType", "multipart");
    add_change_file_parameters(ub);
    add_use_content_as_indexable_text_parameter(ub);
    if (m_add_parents)
        ub.append_query("addParents", *m_add_parents);
    if (m_remove_parents)
        ub.append_query("removeParents", *m_remove_parents);

    std::cerr << "uri: " << ub.to_string() << "\n";

    web::http::http_request r;
    r.set_method(web::http::methods::PATCH);
    r.set_body(body, content_type);
    r.set_request_uri(ub.to_uri());

    return r;
}

}
}
}
