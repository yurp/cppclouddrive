
#include <ccd/http/beast_transport.h>

#include <boost/asio.hpp>
#include <ccd/defs.h>

#include <iostream>

int main()
{
    auto t = ccd::http::beast_transport_factory();

    ccd::http::request r;
    r.method = "GET";
    r.host = "https://www.example.com";
    auto resp = t(r).get();
    //std::cout << resp.body;

    using namespace std::literals;

    boost::asio::io_context io_srv;
    auto th = std::thread([&io_srv]
    {
        boost::asio::io_service::work w { io_srv };
        io_srv.run();
    });

    boost::asio::steady_timer tm { io_srv };
    tm.expires_after(5s);

    std::cout << "1\n";
    tm.async_wait(ccd::ftr).then([](boost::future<void> f)
    {
        f.get();
        std::cout << "It WORKS!\n";
    })
    .get();
    std::cout << "3\n";

    io_srv.stop();
    th.join();

    return 0;
}