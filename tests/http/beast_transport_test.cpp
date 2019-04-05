
#include <ccd/http/beast_transport.h>
#include <ccd/utils.h>

#include <gmock/gmock.h>

#include <iostream>

using ::testing::HasSubstr;

TEST(beast_transport_test, test_example_com)
{
    ccd::http::request r;
    r.method = "GET";
    r.host = "https://example.com";
    r.path = "/";

    auto transport = ccd::http::beast_transport_factory();
    transport(r).then([](ccd::http::response_future f)
    {
        try
        {
            auto resp = f.get();
            ASSERT_EQ(200, resp.code);
            ASSERT_THAT(resp.body, HasSubstr("<title>Example Domain</title>"));
        }
        catch (const ccd::http::exception& e)
        {
            FAIL() << e.http_code() << ", " << e.what();
        }
    }).wait();
}

TEST(beast_transport_test, test_example_com_asio)
{
    boost::asio::io_service ios;
    boost::asio::io_service::work w { ios };

    ccd::http::request r;
    r.method = "GET";
    r.host = "https://example.com";
    r.path = "/";

    ccd::http::async_beast_transport_factory factory { ios };
    auto transport = factory();
    auto f = transport(r).then([&ios](ccd::http::response_future f)
    {
        try
        {
            auto resp = f.get();
            ASSERT_EQ(200, resp.code);
            ASSERT_THAT(resp.body, HasSubstr("<title>Example Domain</title>"));
        }
        catch (const ccd::http::exception& e)
        {
            FAIL() << e.http_code() << ", " << e.what();
        }

        ios.stop();
    });

    ios.run();
    f.wait();
}

TEST(beast_transport_test, test_httpbin_get)
{
    boost::asio::io_service ios;
    boost::asio::io_service::work w { ios };

    ccd::http::request r;
    r.method = "GET";
    r.host = "https://httpbin.org";
    r.path = "/get";

    ccd::http::async_beast_transport_factory factory { ios };
    auto transport = factory();
    auto f = transport(r).then([&ios](ccd::http::response_future f)
    {
        try
        {
            auto resp = f.get();
            ASSERT_EQ(200, resp.code);
            auto js = ccd::from_json(resp.body);

            ASSERT_TRUE(js.is<ccd::var::map_t>());

            ASSERT_TRUE(js.has("url"));
            ASSERT_TRUE(js["url"].is<std::string>());
            ASSERT_EQ(js["url"].as<std::string>(), "https://httpbin.org/get");


            ASSERT_TRUE(js.has("args"));
            ASSERT_TRUE(js["args"].is<ccd::var::map_t>());
            ASSERT_TRUE(js["args"].as<ccd::var::map_t>().empty());

            //ASSERT_THAT(resp.body, HasSubstr("<title>Example Domain</title>"));
        }
        catch (const ccd::http::exception& e)
        {
            FAIL() << e.http_code() << ", " << e.what();
        }
        catch (const std::exception& e)
        {
            FAIL() << e.what();
        }
        catch (...)
        {
            FAIL() << "unknown exception";
        }

        ios.stop();
    });

    using namespace std::literals;

    ios.run_for(10s);
    f.wait();
}