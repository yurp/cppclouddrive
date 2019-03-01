
#include <ccd/gdrive/resource/gdrive_del.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

del::del(boost::shared_future<std::string> token, std::string file_id)
    : m_token(std::move(token))
    , m_file_id(std::move(file_id))
{

}

del& del::set_file_id(std::string x)
{
    m_file_id = std::move(x);
    return *this;
}

boost::future<void> del::exec()
{
    return exec_raw_string().then([](boost::future<std::string> t)
    {
        t.get();
    });
}

boost::future<executor::executor_ptr> del::build_request()
{
    std::shared_ptr<http_executor> e = std::make_shared<cpprest_executor>();
    e->set_method("DELETE");
    e->set_endpoint("https://www.googleapis.com");
    e->append_path("/drive/v3/files");
    e->append_path(m_file_id);
    add_file_parameters(*e);

    return m_token.then([e = std::move(e)](boost::shared_future<std::string> t)
    {
        e->set_oauth2_token(t.get());
        return e;
    });
}

}
}
}
