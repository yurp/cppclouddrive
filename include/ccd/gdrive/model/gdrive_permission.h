
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/defs.h>
#include <ccd/utils.h>

#include <optional>

namespace ccd::gdrive
{
inline namespace v3
{
namespace model
{

class permission
{
public:
    class teamdrive_permission_details
    {
    public:
        teamdrive_permission_details();

        explicit teamdrive_permission_details(ccd::var js);

        web::json::value to_json() const;

        std::optional<std::string> get_teamdrive_permission_type() const;

        std::optional<std::string> get_role() const;

        std::optional<std::string> get_inherited_from() const;

        std::optional<bool> get_inherited() const;

        teamdrive_permission_details& set_teamdrive_permission_type(std::optional<std::string> x);

        teamdrive_permission_details& set_role(std::optional<std::string> x);

        teamdrive_permission_details& set_inherited_from(std::optional<std::string> x);

        teamdrive_permission_details& set_inherited(std::optional<bool> x);

    private:
        ccd::var m_json;
    };

    using teamdrive_permission_details_list_t = std::vector<teamdrive_permission_details>;

    permission();

    explicit permission(ccd::var js);

    ccd::var to_json() const;

    std::optional<std::string> get_id() const;

    std::optional<std::string> get_type() const;

    std::optional<std::string> get_email_address() const;

    std::optional<std::string> get_domain() const;

    std::optional<std::string> get_role() const;

    std::optional<bool> get_allow_file_discovery() const;

    std::optional<std::string> get_display_name() const;

    std::optional<std::string> get_photo_link() const;

    std::optional<timestamp_t> get_expiration_time() const;

    std::optional<teamdrive_permission_details_list_t> get_teamdrive_permission_details() const;

    std::optional<bool> get_deleted() const;

    permission& set_id(std::optional<std::string> x);

    permission& set_type(std::optional<std::string> x);

    permission& set_email_address(std::optional<std::string> x);

    permission& set_domain(std::optional<std::string> x);

    permission& set_role(std::optional<std::string> x);

    permission& set_allow_file_discovery(std::optional<bool> x);

    permission& set_display_name(std::optional<std::string> x);

    permission& set_photo_link(std::optional<std::string> x);

    permission& set_expiration_time(std::optional<std::string> x);

    permission& set_teamdrive_permission_details(std::optional<teamdrive_permission_details_list_t> x);

    permission& set_deleted(std::optional<bool> x);

private:
    ccd::var m_json;
    std::optional<teamdrive_permission_details_list_t> m_teamdrive_permission_details;
};

using permission_list_t = std::vector<permission>;

}
}
}
