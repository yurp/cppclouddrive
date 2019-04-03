
#include <ccd/http/beast_transport.h>

#include <ccd/utils.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <openssl/ssl.h>

#include <iostream>

#include <mutex>
#include <thread>
#include <unordered_set>

namespace ccd::http
{

namespace
{

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

std::once_flag f;

bool is_ssl_short_read_error(boost::system::error_code ec)
{
    static const long short_read_error = 335544539;
    return ec.category() == boost::asio::error::ssl_category && ec.value() == short_read_error;
}

boost::system::error_code process_https_error(boost::system::error_code ec)
{
    if (ec == boost::asio::error::eof || is_ssl_short_read_error(ec))
    {
        // Rationale:
        // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
        std::cerr << "http error " << ec.message() << " processed\n";
        ec.assign(0, ec.category());
    }

    return ec;
}

ccd::use_boost_future_ec_t ftr_https { process_https_error };

void load_root_certificates(ssl::context& ctx)
{
    std::string const cert =
        /*  This is the DigiCert root certificate.

            CN = DigiCert High Assurance EV Root CA
            OU = www.digicert.com
            O = DigiCert Inc
            C = US

            Valid to: Sunday, ?November ?9, ?2031 5:00:00 PM

            Thumbprint(sha1):
            5f b7 ee 06 33 e2 59 db ad 0c 4c 9a e6 d3 8f 1a 61 c7 dc 25
        */
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n"
        "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
        "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n"
        "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n"
        "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n"
        "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n"
        "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n"
        "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n"
        "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n"
        "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n"
        "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n"
        "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n"
        "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n"
        "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n"
        "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n"
        "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n"
        "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n"
        "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n"
        "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n"
        "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n"
        "+OkuE6N36B9K\n"
        "-----END CERTIFICATE-----\n"
    /*  This is the GeoTrust root certificate.

        CN = GeoTrust Global CA
        O = GeoTrust Inc.
        C = US
        Valid to: Friday, ‎May ‎20, ‎2022 9:00:00 PM

        Thumbprint(sha1):
        ‎de 28 f4 a4 ff e5 b9 2f a3 c5 03 d1 a3 49 a7 f9 96 2a 82 12
    */
    ;

    boost::system::error_code ec;
    ctx.add_certificate_authority(boost::asio::buffer(cert.data(), cert.size()), ec);
    if (ec)
    {
        boost::throw_exception(boost::system::system_error { ec });
    }
}

boost::beast::http::request<http::string_body> make_beast_request(const request& r)
{
    using namespace std::literals;

    auto host = r.host.substr(8);

    auto target = r.path.empty() ? "/"s : r.path;
    char c = '?';
    for (const auto& q: r.queries)
    {
        target += c;
        c = '&';
        target += urlencode(q.first) + "=" + urlencode(q.second); // TODO: encode()
    }

    boost::beast::http::request<http::string_body> req{ http::string_to_verb(r.method), target, 11 };
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    for (const auto h: r.headers)
    {
        req.set(h.first, h.second);
    }

    if (!r.body.empty())
    {
        req.body() = r.body;
        req.set(http::field::content_type, r.content_type);
        req.prepare_payload();
    }

    std::cerr << req << "\n\n\n";

    return req;
}

response to_response(const http::response<http::string_body>& res)
{
    ccd::http::response resp;
    resp.body = res.body();
    resp.code = res.result_int();
    std::cerr << resp.body << "\n\n\n";

    return resp;
}

boost::system::error_code set_tlsext_host_name(ssl::stream<tcp::socket>& stream, const std::string& host)
{
    if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
    {
        return boost::system::error_code{ static_cast<int>(::ERR_get_error()),
                                          boost::asio::error::get_ssl_category() };
    }

    return boost::system::error_code { };
}

response_future beast_transport(request r)
{
    return boost::async(boost::launch::async, [r = std::move(r)]
    {
        boost::asio::io_context ioc;
        ssl::context ctx{ ssl::context::sslv23_client };
        load_root_certificates(ctx);

        tcp::resolver resolver{ ioc };
        ssl::stream<tcp::socket> stream{ ioc, ctx };

        auto host = r.host.substr(8);

        if (auto ec = set_tlsext_host_name(stream, host))
        {
            boost::throw_exception(boost::system::system_error{ ec });
        }

        auto const results = resolver.resolve(host.c_str(), "443");
        boost::asio::connect(stream.next_layer(), results.begin(), results.end());
        stream.handshake(ssl::stream_base::client);

        auto req = make_beast_request(r);

        http::write(stream, req);
        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);

        boost::system::error_code ec;
        stream.shutdown(ec);
        if (auto processed_ec = process_https_error(ec))
        {
            throw boost::system::system_error{ processed_ec };
        }

        return to_response(res);
    });
}

response_future async_beast_transport(request r, boost::asio::io_context& ioc)
{
    ssl::context ctx{ ssl::context::sslv23_client };
    load_root_certificates(ctx);
    ctx.set_verify_mode(ssl::verify_peer);

    auto host = r.host.substr(8);

    tcp::resolver resolver { ioc };
    auto stream = std::make_shared<ssl::stream<tcp::socket>>(ioc, ctx);
    auto buffer = std::make_shared<boost::beast::flat_buffer>(); // (Must persist between reads)
    auto resp =   std::make_shared<http::response<http::string_body>>();
    auto res =    std::make_shared<ccd::http::response>();
    auto req =    std::make_shared<boost::beast::http::request<http::string_body>>(make_beast_request(r));

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (auto ec = set_tlsext_host_name(*stream, host))
    {
        return boost::make_exceptional_future<ccd::http::response>(boost::system::system_error{ ec });
    }

    // Look up the domain name
    return resolver.async_resolve(host.c_str(), "443", ftr_https)
        .then([stream](boost::future<tcp::resolver::results_type> f)
        {
            auto results = f.get();
            return boost::asio::async_connect(stream->next_layer(), results.begin(), results.end(), ftr_https);
        })
        .unwrap().then([stream](boost::future<boost::asio::ip::tcp::resolver::iterator> )
        {
            return stream->async_handshake(ssl::stream_base::client, ftr_https);
        })
        .unwrap().then([stream, req](boost::future<void> )
        {
            return http::async_write(*stream, *req, ftr_https);
        })
        .unwrap().then([stream, buffer, resp, req](boost::future<std::size_t> )
        {
            return http::async_read(*stream, *buffer, *resp, ftr_https);
        })
        .unwrap().then([stream, buffer, resp, res](boost::future<std::size_t> )
        {
            *res = to_response(*resp);
            return stream->async_shutdown(ftr_https);
        })
        .unwrap().then([stream, res, buffer](boost::future<void> )
        {
            return *res;
        });
}

}

transport_func beast_transport_factory()
{
    std::call_once(f, [] { OpenSSL_add_ssl_algorithms(); });
    return beast_transport;
}

async_beast_transport_factory::async_beast_transport_factory(boost::asio::io_context& ioc)
    : m_ioc(ioc)
{

}

transport_func async_beast_transport_factory::operator()()
{
    std::call_once(f, [] { OpenSSL_add_ssl_algorithms(); });
    return [ioc = &m_ioc](request r)
    {
        return async_beast_transport(r, *ioc);
    };
}

}
