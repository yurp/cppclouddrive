
#include <ccd/http/beast_transport.h>
#include <ccd/http/curl_transport.h>

#include <openssl/ssl.h>

#include <iostream>

int main()
{
    OpenSSL_add_ssl_algorithms();

    auto t = ccd::http::beast_transport_factory();

    ccd::http::request r;
    r.method = "GET";
    r.host = "http://www.example.com";
    auto resp = t(r).get();
    std::cout << resp.body;

    return 0;
}