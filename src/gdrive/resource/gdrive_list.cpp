
#include <ccd/gdrive/resource/gdrive_list.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

list::list(ccd::http::transport_func http_transport)
    : ccd::details::http_executor(std::move(http_transport))
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

boost::future<model::file_list> list::exec()
{
    return exec_custom<ccd::gdrive::model::file_list>();
}

ccd::http::request list::build_request()
{
    ccd::http::request e;
    
    e.method = "GET";
    e.host = "https://www.googleapis.com";
    e.path = "/drive/v3/files";
    add_file_parameters(e);

    if (m_corpora)
        e.queries.emplace_back("corpora", *m_corpora);
    if (m_corpus)
        e.queries.emplace_back("corpus", *m_corpus);
    if (m_include_team_drive_items)
        e.queries.emplace_back("includeTeamDriveItems", *m_include_team_drive_items ? "true" : "false");
    if (m_order_by)
        e.queries.emplace_back("orderBy", *m_order_by);
    if (m_page_size)
        e.queries.emplace_back("pageSize", std::to_string(*m_page_size));
    if (m_page_token)
        e.queries.emplace_back("pageToken", *m_page_token);
    if (m_q)
        e.queries.emplace_back("q", *m_q);
    if (m_spaces)
        e.queries.emplace_back("spaces", *m_spaces);
    if (m_team_drive_id)
        e.queries.emplace_back("teamDriveId", *m_team_drive_id);

    return e;
}

}
}
}
