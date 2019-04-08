
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
    std::cerr << "@a\n";
    boost::asio::io_service ios;
    boost::asio::io_service::work w { ios };

    std::cerr << "@b\n";
    ccd::http::request r;
    r.method = "GET";
    r.host = "https://httpbin.org";
    r.path = "/get";
    r.queries = { { "q1", "a1" }, { "q2", "a2" }, { "q3", "&&&" } };
    r.headers = { { "Custom-Header-1", "1234567" }, { "Custom-Header-2", "7654321" } };

    std::cerr << "@c\n";
    ccd::http::async_beast_transport_factory factory { ios };
    std::cerr << "@d\n";
    auto transport = factory();
    std::cerr << "@e\n";
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
            ASSERT_EQ(js["url"].as<std::string>(), "https://httpbin.org/get?q1=a1&q2=a2&q3=%26%26%26");

            ASSERT_TRUE(js.has("args"));
            ASSERT_TRUE(js["args"].is<ccd::var::map_t>());
            ASSERT_EQ(js["args"].as<ccd::var::map_t>().size(), 3u);
            ASSERT_EQ(js["args"]["q1"], "a1");
            ASSERT_EQ(js["args"]["q2"], "a2");
            ASSERT_EQ(js["args"]["q3"], "&&&");

            ASSERT_TRUE(js.has("headers"));
            ASSERT_TRUE(js["headers"].is<ccd::var::map_t>());
            ASSERT_EQ(js["headers"].as<ccd::var::map_t>().size(), 4u);
            ASSERT_EQ(js["headers"]["Custom-Header-1"], "1234567");
            ASSERT_EQ(js["headers"]["Custom-Header-2"], "7654321");
            ASSERT_EQ(js["headers"]["Host"], "httpbin.org");
            ASSERT_THAT(js["headers"]["User-Agent"].as<std::string>(), HasSubstr("Boost.Beast/"));
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

    std::cerr << "@f\n";
    ios.run_for(10s);
    std::cerr << "@g\n";
    f.wait();
    std::cerr << "@h\n";
}

TEST(beast_transport_test, test_httpbin_delete_401)
{
    boost::asio::io_service ios;
    boost::asio::io_service::work w { ios };

    ccd::http::request r;
    r.method = "DELETE";
    r.host = "https://httpbin.org";
    r.path = "/status/401";

    ccd::http::async_beast_transport_factory factory { ios };
    auto transport = factory();
    auto f = transport(r).then([&ios](ccd::http::response_future f)
    {
        try
        {
            auto resp = f.get();
            ASSERT_EQ(401, resp.code);
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

TEST(DISABLED_beast_transport_test, test_httpbin_get_cyrillic_header)
{
    boost::asio::io_service ios;
    boost::asio::io_service::work w { ios };

    ccd::http::request r;
    r.method = "GET";
    r.host = "https://httpbin.org";
    r.path = "/get";
    r.queries = { { "q1", "a1" }, { "q2", "a2" }, { "q3", "&&&" } };
    r.headers = { { "Cyrillic-Header", "АБВ" } };

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
            ASSERT_TRUE(js.has("headers"));
            ASSERT_TRUE(js["headers"].is<ccd::var::map_t>());
            ASSERT_EQ(js["headers"]["Cyrillic-Header"], "АБВ");
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


TEST(beast_transport_test, test_httpbin_get_gzip)
{
    boost::asio::io_service ios;
    boost::asio::io_service::work w { ios };

    ccd::http::request r;
    r.method = "GET";
    r.host = "https://httpbin.org";
    r.path = "/get";

    ccd::http::async_beast_transport_factory factory { ios, /* allow_compression = */true };
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
            ASSERT_TRUE(js.has("headers"));
            ASSERT_TRUE(js["headers"].is<ccd::var::map_t>());
            ASSERT_EQ(js["headers"].as<ccd::var::map_t>().size(), 3u);
            ASSERT_EQ(js["headers"]["Accept-Encoding"], "gzip, deflate");
            ASSERT_EQ(js["headers"]["Host"], "httpbin.org");
            ASSERT_THAT(js["headers"]["User-Agent"].as<std::string>(), HasSubstr("Boost.Beast/"));
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

TEST(beast_transport_test, test_httpbin_post)
{
    boost::asio::io_service ios;
    boost::asio::io_service::work w { ios };

    ccd::http::request r;
    r.method = "POST";
    r.host = "https://httpbin.org";
    r.path = "/post";
    r.content_type = "application/json";
    r.body = "{ \"a\": 2 }";

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
            ASSERT_EQ(js["url"], "https://httpbin.org/post");
            ASSERT_TRUE(js.has("headers"));
            ASSERT_TRUE(js["headers"].is<ccd::var::map_t>());
            ASSERT_EQ(js["headers"].as<ccd::var::map_t>().size(), 4u);
            ASSERT_TRUE(js["headers"].has("Content-Length"));
            ASSERT_EQ(js["headers"]["Content-Type"], "application/json");
            ASSERT_EQ(js["headers"]["Host"], "httpbin.org");
            ASSERT_THAT(js["headers"]["User-Agent"].as<std::string>(), HasSubstr("Boost.Beast/"));
            ASSERT_TRUE(js.has("data"));
            ASSERT_EQ(js["data"], "{ \"a\": 2 }");
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
