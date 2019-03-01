
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/utils.h>

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#define BOOST_THREAD_PROVIDES_FUTURE_UNWRAP
#include <boost/thread.hpp>

#include <exception>

namespace ccd
{

class executor_exception : public std::exception
{
public:
    explicit executor_exception(int http_code) noexcept;
    executor_exception(int http_code, std::string msg) noexcept;

    const char* what() const noexcept override;
    int http_code() const noexcept;

private:
    int m_http_code;
    std::string m_msg;
};

class http_executor
{
public:
    virtual ~http_executor() {}

    virtual boost::future<std::string> exec() = 0;

    virtual void reset() = 0;

    virtual void set_endpoint(const std::string& x) = 0;
    virtual void set_method(const std::string& x) = 0;
    virtual void append_path(const std::string& x) = 0;
    virtual void append_query(const std::string& k, const std::string& v) = 0;
    virtual void set_oauth2_token(const std::string& x) = 0;
    virtual void add_header(const std::string& k, const std::string& v) = 0;
    virtual void set_body(const std::string& t, const std::string& x) = 0;
};

using http_executor_ptr = std::shared_ptr<http_executor>;
using http_executor_factory = std::function<http_executor_ptr()>;

class cpprest_executor : public http_executor
{
public:
    boost::future<std::string> exec() override;

    void reset() override;

    void set_endpoint(const std::string& x) override;
    void set_method(const std::string& x) override;
    void append_path(const std::string& x) override;
    void append_query(const std::string& k, const std::string& v) override;
    void set_oauth2_token(const std::string& x) override;
    void add_header(const std::string& k, const std::string& v) override;
    void set_body(const std::string& t, const std::string& x) override;

private:
    std::string m_endpoint;
    std::string m_method;
    std::vector<std::string> m_path;
    std::vector<std::pair<std::string, std::string>> m_queries;
    std::string m_token;
    std::vector<std::pair<std::string, std::string>> m_headers;
    std::pair<std::string, std::string> m_body;

};

class executor
{
public:
    using executor_ptr = std::shared_ptr<http_executor>;

    boost::future<std::string> exec_raw_string();
    boost::future<ccd::var> exec_raw_json();

    template<typename T>
    boost::future<T> exec_custom();

private:
    //template <typename T>
    //static boost::future<T> throw_executor_exception(const web::http::http_response& resp);

    virtual boost::future<executor_ptr> build_request() = 0;
};

template<typename T>
boost::future<T> executor::exec_custom()
{
    return exec_raw_json().then([](boost::future<ccd::var> jsf)
    {
        auto js = jsf.get();
        return T { std::move(js) };
    });
}
#if 0
template <typename T>
pplx::task<T> executor::throw_executor_exception(const web::http::http_response& resp)
{
    return resp.extract_utf8string(true).then([code = resp.status_code()](pplx::task<std::string> body_task)
    {
        auto msg = "code: " + std::to_string(code);
        try
        {
            auto s = body_task.get();
            if (s.length() > 4800)
            {
                s.resize(4797);
                s += "...";
            }
            msg += ", body: " + s;
        }
        catch (...) {}
        return pplx::task_from_exception<T>(executor_exception{ code, msg });
    });
}
#endif
}
