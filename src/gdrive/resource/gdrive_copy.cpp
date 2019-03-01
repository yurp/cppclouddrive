
#include <ccd/gdrive/resource/gdrive_copy.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

copy::copy(boost::shared_future<std::string> token, std::string file_id, model::file metadata_patch)
    : m_token(std::move(token))
    , m_file_id(std::move(file_id))
    , m_metadata_patch(std::move(metadata_patch))
{

}

copy& copy::set_file_id(std::string x)
{
    m_file_id = std::move(x);
    return *this;
}

boost::future<model::file> copy::exec()
{
    return exec_custom<model::file>();
}

boost::future<executor::executor_ptr> copy::build_request()
{
    std::shared_ptr<http_executor> e = std::make_shared<cpprest_executor>();
    e->set_method("POST");
    e->set_endpoint("https://www.googleapis.com");
    e->append_path("drive/v3/files");
    e->append_path(m_file_id);
    e->append_path("copy");
    add_change_file_parameters(*e);
    add_ignore_default_visibility_parameter(*e);
    e->set_body("application/json; charset=utf-8", to_json(m_metadata_patch.to_json()));

    return m_token.then([e = std::move(e)](boost::shared_future<std::string> t)
    {
        e->set_oauth2_token(t.get());
        return e;
    });
}

}
}
}
