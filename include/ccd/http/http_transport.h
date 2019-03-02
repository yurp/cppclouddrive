
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/defs.h>

#include <functional>
#include <exception>

namespace ccd::http
{

class exception : public std::exception
{
public:
    explicit exception(int http_code) noexcept;
    exception(int http_code, std::string msg) noexcept;

    const char* what() const noexcept override;
    int http_code() const noexcept;

private:
    int m_http_code;
    std::string m_msg;
};

struct request
{
    std::string host;
    std::string method;
    std::string path;
    std::vector<std::pair<std::string, std::string>> queries;
    std::vector<std::pair<std::string, std::string>> headers;
    std::string content_type;
    std::string body;
};

struct response
{
    int code;
    std::string body;
};

using response_future = boost::future<response>;
using transport_func = std::function<response_future(request)>;
using transport_factory = std::function<transport_func()>;

}
