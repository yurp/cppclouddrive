
#include <ccd/gdrive/resource/gdrive_del.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

del::del(pplx::task<http_client_ptr> client, std::string file_id)
    : executor(std::move(client))
    , m_file_id(std::move(file_id))
{

}

del& del::set_file_id(std::string x)
{
    m_file_id = std::move(x);
    return *this;
}

pplx::task<void> del::exec()
{
    return exec_raw_string().then([](pplx::task<std::string> t)
                                  {
                                      try
                                      {
                                          t.get();
                                          return pplx::task_from_result<void>();
                                      }
                                      catch (...)
                                      {
                                          return pplx::task_from_exception<void>(std::current_exception());
                                      }
                                  });
}

web::http::http_request del::build_request()
{
    auto ub = web::uri_builder{ }.set_path("/drive/v3/files").append_path(m_file_id);
    add_file_parameters(ub);

    web::http::http_request r;
    r.set_method(web::http::methods::DEL);
    r.set_request_uri(ub.to_uri());

    return r;
}

}
}
}
