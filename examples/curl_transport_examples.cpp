
#include <ccd/http/curl_transport.h>

#include <iostream>

int main()
{
    auto t = ccd::http::curl_transport_factory();

    ccd::http::request r;
    r.method = "GET";
    r.host = "http://www.example.com";
    auto resp = t(r).get();
    std::cout << resp.body;

    return 0;
}