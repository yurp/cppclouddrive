
#include <ccd/gdrive/model/gdrive_permission.h>
#include <ccd/utils.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace model
{

permission::teamdrive_permission_details::teamdrive_permission_details()
    : m_json(web::json::value::object())
{

}

permission::teamdrive_permission_details::teamdrive_permission_details(web::json::value js)
    : m_json(std::move(js))
{

}

web::json::value permission::teamdrive_permission_details::to_json() const
{
    return m_json;
}

std::optional <std::string> permission::teamdrive_permission_details::get_teamdrive_permission_type() const
{
    return details::get_string(m_json, "teamDrivePermissionType");
}

std::optional <std::string> permission::teamdrive_permission_details::get_role() const
{
    return details::get_string(m_json, "role");
}

std::optional <std::string> permission::teamdrive_permission_details::get_inherited_from() const
{
    return details::get_string(m_json, "inheritedFrom");
}

std::optional<bool> permission::teamdrive_permission_details::get_inherited() const
{
    return details::get_bool(m_json, "inherited");
}

permission::teamdrive_permission_details& permission::teamdrive_permission_details::set_teamdrive_permission_type(
    std::optional <std::string> x)
{
    details::set_string(m_json, "teamDrivePermissionType", x);
    return *this;
}

permission::teamdrive_permission_details& permission::teamdrive_permission_details::set_role(
    std::optional <std::string> x)
{
    details::set_string(m_json, "role", x);
    return *this;
}

permission::teamdrive_permission_details& permission::teamdrive_permission_details::set_inherited_from(
    std::optional <std::string> x)
{
    details::set_string(m_json, "inheritedFrom", x);
    return *this;
}

permission::teamdrive_permission_details& permission::teamdrive_permission_details::set_inherited(std::optional<bool> x)
{
    details::set_bool(m_json, "inherited", x);
    return *this;
}

// ---------------------------------------------------------------------------------------------------------------------

permission::permission()
    : m_json(web::json::value::object())
    , m_teamdrive_permission_details(std::nullopt)
{

}

permission::permission(web::json::value js)
    : m_json(std::move(js))
    , m_teamdrive_permission_details(
        details::create_object_list<teamdrive_permission_details>(m_json, "teamDrivePermissionDetails"))
{

}

web::json::value permission::to_json() const
{
    auto js = m_json;
    details::object_list_to_json(js, "teamDrivePermissionDetails", m_teamdrive_permission_details);

    return js;
}

std::optional <std::string> permission::get_id() const
{
    return details::get_string(m_json, "id");
}

std::optional <std::string> permission::get_type() const
{
    return details::get_string(m_json, "type");
}

std::optional <std::string> permission::get_email_address() const
{
    return details::get_string(m_json, "emailAddress");
}

std::optional <std::string> permission::get_domain() const
{
    return details::get_string(m_json, "domain");
}

std::optional <std::string> permission::get_role() const
{
    return details::get_string(m_json, "role");
}

std::optional<bool> permission::get_allow_file_discovery() const
{
    return details::get_bool(m_json, "allowFileDiscovery");
}

std::optional <std::string> permission::get_display_name() const
{
    return details::get_string(m_json, "displayName");
}

std::optional <std::string> permission::get_photo_link() const
{
    return details::get_string(m_json, "photoLink");
}

std::optional <timestamp_t> permission::get_expiration_time() const
{
    return details::get_string(m_json, "expirationTime");
}

std::optional <permission::teamdrive_permission_details_list_t> permission::get_teamdrive_permission_details() const
{
    return m_teamdrive_permission_details;
}

std::optional<bool> permission::get_deleted() const
{
    return details::get_bool(m_json, "deleted");
}

permission& permission::set_id(std::optional <std::string> x)
{
    details::set_string(m_json, "id", x);
    return *this;
}

permission& permission::set_type(std::optional <std::string> x)
{
    details::set_string(m_json, "type", x);
    return *this;
}

permission& permission::set_email_address(std::optional <std::string> x)
{
    details::set_string(m_json, "emailAddress", x);
    return *this;
}

permission& permission::set_domain(std::optional <std::string> x)
{
    details::set_string(m_json, "domain", x);
    return *this;
}

permission& permission::set_role(std::optional <std::string> x)
{
    details::set_string(m_json, "role", x);
    return *this;
}

permission& permission::set_allow_file_discovery(std::optional<bool> x)
{
    details::set_bool(m_json, "allowFileDiscovery", x);
    return *this;
}

permission& permission::set_display_name(std::optional <std::string> x)
{
    details::set_string(m_json, "displayName", x);
    return *this;
}

permission& permission::set_photo_link(std::optional <std::string> x)
{
    details::set_string(m_json, "photoLink", x);
    return *this;
}

permission& permission::set_expiration_time(std::optional <std::string> x)
{
    details::set_string(m_json, "expirationTime", x);
    return *this;
}

permission& permission::set_teamdrive_permission_details(
    std::optional <permission::teamdrive_permission_details_list_t> x)
{
    m_teamdrive_permission_details = std::move(x);
    return *this;
}

permission& permission::set_deleted(std::optional<bool> x)
{
    details::set_bool(m_json, "deleted", x);
    return *this;
}

}
}
}
