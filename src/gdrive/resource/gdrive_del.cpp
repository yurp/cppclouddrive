
#include <ccd/gdrive/resource/gdrive_del.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

del::del(ccd::http::transport_func http_transport, std::string file_id)
    : ccd::details::http_executor(std::move(http_transport))
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

ccd::http::request del::build_request()
{
    ccd::http::request e;
    e.method = "DELETE";
    e.host = "https://www.googleapis.com";
    e.path = "/drive/v3/files/" + m_file_id;
    add_file_parameters(e);

    return e;
}

}
}
}
