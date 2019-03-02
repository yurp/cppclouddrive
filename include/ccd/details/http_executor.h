
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/utils.h>
#include <ccd/http/http_transport.h>

namespace ccd::details
{

class http_executor
{
public:
    explicit http_executor(ccd::http::transport_func transport) : m_transport(std::move(transport)) { }
    boost::future<std::string> exec_raw_string();
    boost::future<ccd::var> exec_raw_json();

    template<typename T>
    boost::future<T> exec_custom()
    {
        return exec_raw_json().then([](boost::future<ccd::var> js)
        {
            return T { js.get() };
        });
    }

private:
    virtual ccd::http::request build_request() = 0;

    ccd::http::transport_func m_transport;
};

}
