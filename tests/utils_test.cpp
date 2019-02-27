
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
                                  "a":[1, 2, 3, 4]
                               })");

    ASSERT_TRUE(c.is<ccd::container::map_t>());
    ASSERT_EQ(c.as<ccd::container::map_t>().size(), 7u);

    ASSERT_EQ(c.as<ccd::container::map_t>().count("hello"), 1);
    ASSERT_TRUE(c["hello"].is<ccd::container::string_t>());
    ASSERT_EQ(c["hello"].as<ccd::container::string_t>(), "world");

    ASSERT_EQ(m.count("t"), 1);
    const auto& t = m.at("t");
    ASSERT_TRUE(std::holds_alternative<ccd::container::bool_t>(t.value));
    ASSERT_TRUE(std::get<ccd::container::bool_t>(t.value));

    ASSERT_EQ(m.count("f"), 1);
    const auto& f = m.at("f");
    ASSERT_TRUE(std::holds_alternative<ccd::container::bool_t>(f.value));
    ASSERT_FALSE(std::get<ccd::container::bool_t>(f.value));

    ASSERT_EQ(m.count("i"), 1);
    const auto& i = m.at("i");
    ASSERT_TRUE(std::holds_alternative<ccd::container::int_t>(i.value));
    ASSERT_EQ(std::get<ccd::container::int_t>(i.value), 123);

    ASSERT_EQ(m.count("pi"), 1);
    const auto& pi = m.at("pi");
    ASSERT_TRUE(std::holds_alternative<ccd::container::double_t>(pi.value));
    ASSERT_DOUBLE_EQ(std::get<ccd::container::double_t>(pi.value), 3.1416);

    ASSERT_EQ(m.count("a"), 1);
    ASSERT_TRUE(std::holds_alternative<ccd::container::vector_t>(m.at("a").value));
    const auto& a = std::get<ccd::container::vector_t>(m.at("a").value);

    ASSERT_EQ(a.size(), 4u);
    ASSERT_TRUE(std::holds_alternative<ccd::container::int_t>(a[0].value));
    ASSERT_EQ(std::get<ccd::container::int_t>(a[0].value), 1);

    ASSERT_TRUE(std::holds_alternative<ccd::container::int_t>(a[1].value));
    ASSERT_EQ(std::get<ccd::container::int_t>(a[1].value), 2);

    ASSERT_TRUE(std::holds_alternative<ccd::container::int_t>(a[2].value));
    ASSERT_EQ(std::get<ccd::container::int_t>(a[2].value), 3);

    ASSERT_TRUE(std::holds_alternative<ccd::container::int_t>(a[3].value));
    ASSERT_EQ(std::get<ccd::container::int_t>(a[3].value), 4);
}
