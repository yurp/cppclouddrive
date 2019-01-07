
#include <ccd/gdrive/model/gdrive_user.h>
#include <ccd/utils.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace model
{

user::user()
    : m_json(web::json::value::object())
{

}

user::user(web::json::value js)
    : m_json(std::move(js))
{

}

web::json::value user::to_json() const
{
    return m_json;
}

std::optional <std::string> user::get_display_name() const
{
    return details::get_string(m_json, "displayName");
}

std::optional <std::string> user::get_photo_link() const
{
    return details::get_string(m_json, "photoLink");
}

std::optional<bool> user::get_me() const
{
    return details::get_bool(m_json, "me");
}

std::optional <std::string> user::get_permission_id() const
{
    return details::get_string(m_json, "permissionId");
}

std::optional <std::string> user::get_email_address() const
{
    return details::get_string(m_json, "emailAddress");
}

user& user::set_display_name(std::optional <std::string> x)
{
    details::set_string(m_json, "displayName", x);
    return *this;
}

user& user::set_photo_link(std::optional <std::string> x)
{
    details::set_string(m_json, "photoLink", x);
    return *this;
}

user& user::set_me(std::optional<bool> x)
{
    details::set_bool(m_json, "me", x);
    return *this;
}

user& user::set_permission_id(std::optional <std::string> x)
{
    details::set_string(m_json, "permissionId", x);
    return *this;
}

user& user::set_email_address(std::optional <std::string> x)
{
    details::set_string(m_json, "emailAddress", x);
    return *this;
}

}
}
}
