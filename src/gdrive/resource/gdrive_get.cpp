
#include <ccd/gdrive/resource/gdrive_get.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

get::get(boost::shared_future<std::string> token, std::string file_id)
    : m_token(std::move(token))
    , m_file_id(std::move(file_id))
{

}

get& get::set_file_id(std::string file_id)
{
    m_file_id = std::move(file_id);
    return *this;
}

get& get::set_acknowledge_abuse(std::optional<bool> x)
{
    m_acknowledge_abuse = std::move(x);
    return *this;
}

boost::future<std::string> get::exec_media()
{
    set_alt("media");
    m_range = std::nullopt;
    return exec_raw_string();
}

boost::future<std::string> get::exec_media(int64_t offset, int64_t sz)
{
    set_alt("media");
    m_range = std::make_pair(offset, offset + sz - 1);
    return exec_raw_string();
}

boost::future<model::file> get::exec()
{
    set_alt(std::nullopt);
    m_range = std::nullopt;
    return exec_custom<model::file>();
}

boost::future<executor::executor_ptr> get::build_request()
{
    std::shared_ptr<http_executor> e = std::make_shared<cpprest_executor>();
    e->set_method("GET");
    e->set_endpoint("https://www.googleapis.com");
    e->append_path("/drive/v3/files");
    e->append_path(m_file_id);
    add_file_parameters(*e);

    if (m_acknowledge_abuse)
        e->append_query("acknowledgeAbuse", *m_acknowledge_abuse ? "true" : "false");

    if (m_range)
        e->add_header("Range", "bytes=" + std::to_string(m_range->first) + "-" + std::to_string(m_range->second));

    return m_token.then([e = std::move(e)](boost::shared_future<std::string> t)
    {
        e->set_oauth2_token(t.get());
        return e;
    });
}

}
}
}
