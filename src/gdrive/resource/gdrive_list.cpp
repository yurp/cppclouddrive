
#include <ccd/gdrive/resource/gdrive_list.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

list::list(boost::shared_future<std::string> token)
    : m_token(std::move(token))
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

boost::future<executor::executor_ptr> list::build_request()
{
    std::shared_ptr<http_executor> e = std::make_shared<cpprest_executor>();
    e->set_method("GET");
    e->set_endpoint("https://www.googleapis.com");
    e->append_path("/drive/v3/files");
    add_file_parameters(*e);

    if (m_corpora)
        e->append_query("corpora", *m_corpora);
    if (m_corpus)
        e->append_query("corpus", *m_corpus);
    if (m_include_team_drive_items)
        e->append_query("includeTeamDriveItems", *m_include_team_drive_items ? "true" : "false");
    if (m_order_by)
        e->append_query("orderBy", *m_order_by);
    if (m_page_size)
        e->append_query("pageSize", std::to_string(*m_page_size));
    if (m_page_token)
        e->append_query("pageToken", *m_page_token);
    if (m_q)
        e->append_query("q", *m_q);
    if (m_spaces)
        e->append_query("spaces", *m_spaces);
    if (m_team_drive_id)
        e->append_query("teamDriveId", *m_team_drive_id);

    return m_token.then([e = std::move(e)](boost::shared_future<std::string> t)
    {
        e->set_oauth2_token(t.get());
        return e;
    });
}

}
}
}
