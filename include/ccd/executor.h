
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/defs.h>

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

class executor
{
public:
    explicit executor(pplx::task<http_client_ptr> client_task);

    pplx::task<std::string> exec_raw_string();
    pplx::task<web::json::value> exec_raw_json();

    template<typename T>
    pplx::task<T> exec_custom();

private:
    template <typename T, pplx::task<T>(web::http::http_response::*func)(bool) const>
    pplx::task<T> exec_raw();

    virtual web::http::http_request build_request() = 0;

    pplx::task<http_client_ptr> m_client_task;
};

template<typename T>
pplx::task<T> executor::exec_custom()
{
    return exec_raw_json().then([](pplx::task<web::json::value> js_task)
    {
        try
        {
            auto js = js_task.get();
            return pplx::task_from_result(T { std::move(js) });
        }
        catch (...)
        {
            return pplx::task_from_exception<T>(std::current_exception());
        }
    });
}

template <typename T, pplx::task<T>(web::http::http_response::*func)(bool) const>
pplx::task<T> executor::exec_raw()
{
    return m_client_task.then([r = build_request()](http_client_ptr c)
    {
        return c->request(r);
    })
    .then([](pplx::task<web::http::http_response> resp_task)
    {
        try
        {
            auto resp = resp_task.get();
            auto code = resp.status_code();
            return  (code >= 200 && code < 300) ? (resp.*func)(true)
                                                : pplx::task_from_exception<T>(executor_exception{ code });
        }
        catch (...)
        {
            return pplx::task_from_exception<T>(std::current_exception());
        }
    });
}

}
