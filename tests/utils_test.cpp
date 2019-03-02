
#include <ccd/utils.h>
#include <gtest/gtest.h>

TEST(utils_test, parse_json_from_string)
{
    auto c = ccd::from_json(R"({
                                  "hello" : "world",
                                  "t" : true ,
                                  "f" : false,
                                  "n": null,
                                  "i":123,
                                  "pi": 3.1416,
                                  "a":[1, 2, 3, 4, "42", [], {}, [ 1 ] ]
                               })");

    ASSERT_TRUE(c.is<ccd::var::map_t>());
    ASSERT_EQ(c.as<ccd::var::map_t>().size(), 7u);

    ASSERT_EQ(c.has("hello"), 1);
    ASSERT_TRUE(c["hello"].is<ccd::var::string_t>());
    ASSERT_EQ(c["hello"].as<ccd::var::string_t>(), "world");

    ASSERT_EQ(c.has("t"), 1);
    ASSERT_TRUE(c["t"].is<ccd::var::bool_t>());
    ASSERT_TRUE(c["t"].as<ccd::var::bool_t>());

    ASSERT_EQ(c.has("f"), 1);
    ASSERT_TRUE(c["f"].is<ccd::var::bool_t>());
    ASSERT_FALSE(c["f"].as<ccd::var::bool_t>());

    ASSERT_EQ(c.has("i"), 1);
    ASSERT_TRUE(c["i"].is<ccd::var::int_t>());
    ASSERT_EQ(c["i"].as<ccd::var::int_t>(), 123);

    ASSERT_EQ(c.has("pi"), 1);
    ASSERT_TRUE(c["pi"].is<ccd::var::double_t>());
    ASSERT_EQ(c["pi"].as<ccd::var::double_t>(), 3.1416);

    ASSERT_EQ(c.has("a"), 1);
    ASSERT_TRUE(c["a"].is<ccd::var::vector_t>());

    ASSERT_EQ(c["a"].as<ccd::var::vector_t>().size(), 8u);

    ASSERT_TRUE(c["a"][0].is<ccd::var::int_t>());
    ASSERT_EQ(c["a"][0].as<ccd::var::int_t>(), 1);

    ASSERT_TRUE(c["a"][1].is<ccd::var::int_t>());
    ASSERT_EQ(c["a"][1].as<ccd::var::int_t>(), 2);

    ASSERT_TRUE(c["a"][2].is<ccd::var::int_t>());
    ASSERT_EQ(c["a"][2].as<ccd::var::int_t>(), 3);

    ASSERT_TRUE(c["a"][3].is<ccd::var::int_t>());
    ASSERT_EQ(c["a"][3].as<ccd::var::int_t>(), 4);

    ASSERT_TRUE(c["a"][4].is<ccd::var::string_t>());
    ASSERT_EQ(c["a"][4].as<ccd::var::string_t>(), "42");

    ASSERT_TRUE(c["a"][5].is<ccd::var::vector_t>());
    ASSERT_TRUE(c["a"][5].as<ccd::var::vector_t>().empty());

    ASSERT_TRUE(c["a"][6].is<ccd::var::map_t>());
    ASSERT_TRUE(c["a"][6].as<ccd::var::map_t>().empty());

    ASSERT_TRUE(c["a"][7].is<ccd::var::vector_t>());
    ASSERT_EQ(c["a"][7].as<ccd::var::vector_t>().size(), 1);
    ASSERT_EQ(c["a"][7][0].as<ccd::var::int_t>(), 1);
}

TEST(utils_test, parse_json_to_string)
{
    ccd::var v;
    v =  ccd::var::map_t{};
    v["name"] = "euler.cpp";
    v["size"] = 42;
    v["attr"] = ccd::var::vector_t{};
    v["attr"].as<ccd::var::vector_t>().emplace_back(100);
    v["attr"].as<ccd::var::vector_t>().emplace_back(99.579);
    v["attr"].as<ccd::var::vector_t>().emplace_back(ccd::var::map_t{});
    v["attr"][2]["lon"] = 999123;
    v["attr"][2]["lat"] = 7999123;

    auto s = ccd::to_json(v);
    std::cerr << s << "\n";
}

