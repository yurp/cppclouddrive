
#include <ccd/http/cpprest_transport.h>

#include <cpprest/http_client.h>

namespace ccd::http
{

namespace
{

web::http::http_request build_request(const request& r)
{
    using namespace utility::conversions;

    web::uri_builder ub;
    ub.append_path(to_string_t(r.path));
    for (const auto& q: r.queries)
    {
        ub.append_query(to_string_t(q.first), to_string_t(q.second));
    }

    web::http::http_request req { to_string_t(r.method) };
    for (const auto& h: r.headers)
    {
        req.headers().add(to_string_t(h.first), to_string_t(h.second));
    }
    if (!r.body.empty())
    {
        req.set_body(r.body, r.content_type);
    }
    req.set_request_uri(ub.to_uri());

    return req;
}

response_future cpprest_transport(request r)
{
    using namespace utility::conversions;

    auto hr = std::make_shared<response>();
    boost::promise<response> p;
    auto f = p.get_future();

    web::http::client::http_client c { to_string_t(r.host) };
    auto t = c.request(build_request(r)).then([hr](web::http::http_response resp)
    {
        hr->code = resp.status_code();
        return resp.extract_utf8string(true);
    })
    .then([hr](std::string s)
    {
        hr->body = std::move(s);
        return 0;
    })
    .then([hr, p = std::move(p)](pplx::task<int> i) mutable
    {
        try
        {
            i.get(); // if exception occured it will throw here
            p.set_value(std::move(*hr));
        }
        catch (...)
        {
            p.set_exception(boost::current_exception()); // TODO: looks like set_exception() can throw
            //p.set_exception(std::current_exception()); // TODO: looks like set_exception() can throw
        }
    });

    return f;
}

}

transport_func cpprest_transport_factory()
{
    return cpprest_transport;
}

}
