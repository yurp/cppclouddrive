
#include <ccd/http/beast_transport.h>

#include <ccd/utils.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>

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
    if (ec)
    {
        std::cerr << ">>>>>>>>>http error " << ec.message() << " processed\n";
    }

    return ec;
}

void load_root_certificates(ssl::context& ctx)
{
    static const std::string cert =
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
    std::cerr << "%%%1\n";
    ctx.add_certificate_authority(boost::asio::buffer(cert.data(), cert.size()), ec);
    std::cerr << "%%%2\n";
    if (ec)
    {
        std::cerr << "cert auth error: " << ec.message() << "\n";
        boost::throw_exception(boost::system::system_error { ec });
    }
}

ssl::context& create_ssl_ctx()
{
    static auto ctx = []
    {
        std::cerr << "$$$1\n";
        ssl::context ctx{ ssl::context::sslv23_client };
        std::cerr << "$$$2\n";
        load_root_certificates(ctx);
        std::cerr << "$$$3\n";
        ctx.set_verify_mode(ssl::verify_peer);
        std::cerr << "$$$4\n";

        return ctx;
    }();

    std::cerr << "^^^1\n";
    return ctx;
}

boost::beast::http::request<http::string_body> make_beast_request(const request& r, bool allow_compression)
{
    std::cerr << "#\n";
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
    if (allow_compression)
    {
        req.set(http::field::accept_encoding, "gzip, deflate");
    }

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

    std::cerr << "\nREQ---------------------------------------------------------------\n\n"
              << req
              << "\n------------------------------------------------------------------\n\n";

    return req;
}

response to_response(const http::response<http::string_body>& res)
{
    ccd::http::response resp;
    resp.body = res.body();
    resp.code = res.result_int();

    //decompress with boost's interface
    boost::iostreams::array_source src { resp.body.data(), resp.body.size() };
    boost::iostreams::filtering_istream is;

     //boost::iostreams::zlib_params zparams{};
    if (res["Content-Encoding"] == "deflate")
    {
        std::cerr << "decompressing " << res["Content-Encoding"] << std::endl;
        std::cerr << "-------------------------------------------------" << '\n';
        //is.push(boost::iostreams::zlib_decompressor{ -MAX_WBITS });			// deflate
        is.push(boost::iostreams::zlib_decompressor{ });			// deflate
    }
    else if (res["Content-Encoding"] == "gzip")
    {
        std::cerr << "decompressing " << res["Content-Encoding"] << std::endl;
        std::cerr << "-------------------------------------------------" << '\n';
        is.push(boost::iostreams::gzip_decompressor{});						// gzip
    }
    else if (res["Content-Encoding"] == "")
    {
        std::cerr << "uncompressed " << res["Content-Encoding"] << std::endl;
        std::cerr << "-------------------------------------------------" << '\n';
    }

    is.push(src);
    std::stringstream ss;
    boost::iostreams::copy(is, ss);
    resp.body = ss.str();

    std::cerr << "\nRES---------------------------------------------------------------\n\n"
              << res
              << "\n------------------------------------------------------------------\n\n";

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

class data
{
public:
    //ssl::context m_ctx;
    tcp::resolver m_resolver;
    ssl::stream<tcp::socket> m_stream;
    boost::beast::flat_buffer m_buffer;
    http::request<http::string_body> m_beast_request;
    http::response<http::string_body> m_beast_response;

    data(request r, boost::asio::io_context& ioc, bool allow_compression)
        //: m_ctx(create_ssl_ctx())
        : m_resolver(ioc)
        , m_stream(ioc, create_ssl_ctx()/*m_ctx*/)
        , m_buffer()
        , m_beast_request(make_beast_request(std::move(r), allow_compression))
        , m_beast_response()
    {

    }

    boost::system::error_code prepare()
    {
        return set_tlsext_host_name(m_stream, m_beast_request[http::field::host].to_string());
    }
};

response_future beast_transport(request r)
{
    return boost::async(boost::launch::async, [r = std::move(r)]
    {
        boost::asio::io_context ioc;
        data d { std::move(r), ioc, false };

        if (auto ec = d.prepare())
        {
            boost::throw_exception(boost::system::system_error{ ec });
        }

        auto host = d.m_beast_request[http::field::host].to_string();
        auto const results = d.m_resolver.resolve(host.c_str(), "443");
        boost::asio::connect(d.m_stream.next_layer(), results.begin(), results.end());
        d.m_stream.handshake(ssl::stream_base::client);
        http::write(d.m_stream, d.m_beast_request);
        http::read(d.m_stream, d.m_buffer, d.m_beast_response);

        boost::system::error_code ec;
        d.m_stream.shutdown(ec);
        if (auto processed_ec = process_https_error(ec))
        {
            boost::throw_exception(boost::system::system_error{ processed_ec });
        }

        return to_response(d.m_beast_response);
    });
}

ccd::use_boost_future_ec_t ftr_https { process_https_error };

response_future async_beast_transport(request r, boost::asio::io_context& ioc, bool allow_compression)
{
    std::cerr << "!0\n";
    auto d = std::make_shared<data>(std::move(r), ioc, allow_compression);

    std::cerr << "!1\n";

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (auto ec = d->prepare())
    {
        return boost::make_exceptional_future<ccd::http::response>(boost::system::system_error{ ec });
    }

    std::cerr << "!2\n";

    // Look up the domain name
    auto host = d->m_beast_request[http::field::host].to_string();
    return d->m_resolver.async_resolve(host.c_str(), "443", ftr_https)
        .then([d](boost::future<tcp::resolver::results_type> f)
        {
            std::cerr << "!3\n";
            auto results = f.get();
            return boost::asio::async_connect(d->m_stream.next_layer(), results.begin(), results.end(), ftr_https);
        })
        .unwrap().then([d](boost::future<boost::asio::ip::tcp::resolver::iterator> )
        {
            std::cerr << "!4\n";
            return d->m_stream.async_handshake(ssl::stream_base::client, ftr_https);
        })
        .unwrap().then([d](boost::future<void> )
        {
            std::cerr << "!5\n";
            return http::async_write(d->m_stream, d->m_beast_request, ftr_https);
        })
        .unwrap().then([d](boost::future<std::size_t> )
        {
            std::cerr << "!6\n";
            return http::async_read(d->m_stream, d->m_buffer, d->m_beast_response, ftr_https);
        })
        .unwrap().then([d](boost::future<std::size_t> )
        {
            std::cerr << "!7\n";
            return d->m_stream.async_shutdown(ftr_https);
        })
        .unwrap().then([d](boost::future<void> )
        {
            std::cerr << "!8\n";
            return to_response(d->m_beast_response);
        });
}

}

transport_func beast_transport_factory()
{
    std::call_once(f, [] { OpenSSL_add_ssl_algorithms(); });
    return beast_transport;
}

async_beast_transport_factory::async_beast_transport_factory(boost::asio::io_context& ioc, bool allow_compression)
    : m_ioc(ioc)
    , m_allow_compression(allow_compression)
{

}

transport_func async_beast_transport_factory::operator()()
{
    std::call_once(f, [] { OpenSSL_add_ssl_algorithms(); });
    return [ioc = &m_ioc, allow_compression = m_allow_compression](request r)
    {
        std::cerr << "!-0\n";
        return async_beast_transport(r, *ioc, allow_compression);
    };
}

}
