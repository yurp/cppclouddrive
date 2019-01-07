
#include <ccd/executor.h>

namespace ccd
{

executor_exception::executor_exception(int http_code) noexcept
    : m_http_code(http_code)
    , m_msg("http request failed with code " + std::to_string(m_http_code))
{

}

executor_exception::executor_exception(int http_code, std::string msg) noexcept
    : m_http_code(http_code)
    , m_msg(std::move(msg))
{

}

const char* executor_exception::what() const noexcept
{
return m_msg.c_str();
}

int executor_exception::http_code() const noexcept
{
    return m_http_code;
}

// ---------------------------------------------------------------------------------------------------------------------

executor::executor(pplx::task<http_client_ptr> client_task)
    : m_client_task{ std::move(client_task) }
{

}

pplx::task<std::string> executor::exec_raw_string()
{
    return exec_raw<std::string, &web::http::http_response::extract_utf8string>();
}

pplx::task<web::json::value> executor::exec_raw_json()
{
    return exec_raw<web::json::value, &web::http::http_response::extract_json>();
}

}