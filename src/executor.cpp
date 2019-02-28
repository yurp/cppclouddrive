
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

boost::future<std::string> cpprest_executor::exec()
{
    using namespace utility::conversions;

    boost::promise<std::string> p;
    auto f = p.get_future();

    web::uri_builder ub;
    for (const auto p: m_path)
    {
        ub.append_path(to_string_t(p));
    }
    for (const auto q: m_queries)
    {
        ub.append_query(to_string_t(q.first), to_string_t(q.second));
    }

    web::http::http_request r { to_string_t(m_method) };
    r.headers().add(to_string_t("Authorization"), to_string_t("Bearer " + m_token));
    for (const auto h: m_headers)
    {
        r.headers().add(to_string_t(h.first), to_string_t(h.second));
    }
    r.set_body(m_body.second, m_body.first);
    r.set_request_uri(ub.to_uri());

    web::http::client::http_client c { to_string_t(m_endpoint) };
    c.request(r).then([](pplx::task<web::http::http_response> resp)
    {
        return resp.get().extract_utf8string(true);
    })
    .then([p = std::move(p)](pplx::task<std::string> s) mutable
    {
        p.set_value(s.get());
    });

    return f;
}

void cpprest_executor::reset()
{
    m_endpoint = "";
    m_method = "";
    m_path.clear();
    m_queries.clear();
    m_token = "";
    m_headers.clear();
    m_body = { "", "" };
}

void cpprest_executor::set_endpoint(const std::string& x)
{
    m_endpoint = std::move(x);
}

void cpprest_executor::set_method(const std::string& x)
{
    m_method = std::move(x);
}

void cpprest_executor::append_path(const std::string& x)
{
    m_path.emplace_back(std::move(x));
}

void cpprest_executor::append_query(const std::string& k, const std::string& v)
{
    m_queries.emplace_back(std::move(k), std::move(v));
}

void cpprest_executor::set_oauth2_token(const std::string& x)
{
    m_token = std::move(x);
}

void cpprest_executor::add_header(const std::string& k, const std::string& v)
{
    m_headers.emplace_back(std::move(k), std::move(v));
}

void cpprest_executor::set_body(const std::string& t, const std::string& x)
{
    m_body = { std::move(t), std::move(x) };
}


// ---------------------------------------------------------------------------------------------------------------------

boost::future<std::string> executor::exec_raw_string()
{
    return build_request().then([](boost::future<executor_ptr> e)
    {
        return e.get()->exec();
    }).unwrap();
}

boost::future<ccd::var> executor::exec_raw_json()
{
    return exec_raw_string().then([](boost::future<std::string> s)
    {
        return ccd::from_json(s.get());
    });
}

}