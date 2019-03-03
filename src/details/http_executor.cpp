
#include <ccd/details/http_executor.h>

#include <iostream>

namespace ccd::details
{

boost::future<std::string> http_executor::exec_raw_string()
{
    return m_transport(build_request()).then([](ccd::http::response_future rf)
    {
        auto r = rf.get();
        if (r.code < 200 || r.code >= 300)
        {
            boost::throw_exception(ccd::http::exception { r.code, r.body });
        }

        return r.body;
    });
}

boost::future<ccd::var> http_executor::exec_raw_json()
{
    return exec_raw_string().then([](boost::future<std::string> s)
    {
        return ccd::from_json(s.get());
    });
}

}
