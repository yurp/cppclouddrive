
#include <ccd/gdrive/model/gdrive_user.h>
#include <gtest/gtest.h>

TEST(gdrive_user_test, build_from_json_with_email_and_check)
{
    auto js_str = R"js({ "emailAddress": "example@example.com" })js";
    auto js = ccd::from_json(js_str);
    ccd::gdrive::model::user u { js };

    auto email = u.get_email_address();
    ASSERT_TRUE(email);
    ASSERT_EQ(*email, "example@example.com");

    u.set_email_address(std::nullopt);
    ASSERT_FALSE(u.get_email_address());
}
