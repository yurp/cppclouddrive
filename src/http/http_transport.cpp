
#include <ccd/http/http_transport.h>

namespace ccd::http
{

exception::exception(int http_code) noexcept
    : m_http_code(http_code)
    , m_msg("http request failed with code " + std::to_string(m_http_code))
{

}

exception::exception(int http_code, std::string msg) noexcept
    : m_http_code(http_code)
    , m_msg(std::move(msg))
{

}

const char* exception::what() const noexcept
{
    return m_msg.c_str();
}

int exception::http_code() const noexcept
{
    return m_http_code;
}

}
