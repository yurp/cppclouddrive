
#include <ccd/gdrive/resource/gdrive_list.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

list::list(pplx::task<http_client_ptr> client)
    : executor(std::move(client))
{

}

list& list::set_corpora(std::optional<std::string> x)
{
    m_corpora = std::move(x);
    return *this;
}

list& list::set_corpus(std::optional<std::string> x)
{
    m_corpus = std::move(x);
    return *this;
}

list& list::set_include_team_drive_items(std::optional<bool> x)
{
    m_include_team_drive_items = std::move(x);
    return *this;
}

list& list::set_order_by(std::optional<std::string> x)
{
    m_order_by = std::move(x);
    return *this;
}

list& list::set_page_size(std::optional<int> x)
{
    m_page_size = std::move(x);
    return *this;
}

list& list::set_page_token(std::optional<std::string> x)
{
    m_page_token = std::move(x);
    return *this;
}

list& list::set_q(std::optional<std::string> x)
{
    m_q = std::move(x);
    return *this;
}

list& list::set_spaces(std::optional<std::string> x)
{
    m_spaces = std::move(x);
    return *this;
}

list& list::set_team_drive_id(std::optional<std::string> x)
{
    m_team_drive_id = std::move(x);
    return *this;
}

pplx::task<model::file_list> list::exec()
{
    return exec_custom<ccd::gdrive::model::file_list>();
}

web::http::http_request list::build_request()
{
    auto ub = web::uri_builder{ }.set_path("/drive/v3/files");
    add_file_parameters(ub);

    if (m_corpora)
        ub.append_query("corpora", *m_corpora);
    if (m_corpus)
        ub.append_query("corpus", *m_corpus);
    if (m_include_team_drive_items)
        ub.append_query("includeTeamDriveItems", *m_include_team_drive_items ? "true" : "false");
    if (m_order_by)
        ub.append_query("orderBy", *m_order_by);
    if (m_page_size)
        ub.append_query("pageSize", *m_page_size);
    if (m_page_token)
        ub.append_query("pageToken", *m_page_token);
    if (m_q)
        ub.append_query("q", *m_q);
    if (m_spaces)
        ub.append_query("spaces", *m_spaces);
    if (m_team_drive_id)
        ub.append_query("teamDriveId", *m_team_drive_id);

    web::http::http_request r;
    r.set_request_uri(ub.to_uri());

    return r;
}

}
}
}
