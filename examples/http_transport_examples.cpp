
#include <ccd/http/beast_transport.h>

#include <iostream>

int main()
{
    auto t = ccd::http::beast_transport_factory();

    ccd::http::request r;
    r.method = "GET";
    r.host = "https://www.example.com";
    auto resp = t(r).get();
    //std::cout << resp.body;

    return 0;
}