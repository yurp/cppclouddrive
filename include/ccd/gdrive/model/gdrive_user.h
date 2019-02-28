
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

class user
{
public:
    user();

    explicit user(ccd::var js);

    ccd::var to_json() const;

    std::optional<std::string> get_display_name() const;

    std::optional<std::string> get_photo_link() const;

    std::optional<bool> get_me() const;

    std::optional<std::string> get_permission_id() const;

    std::optional<std::string> get_email_address() const;

    user& set_display_name(std::optional<std::string> x);

    user& set_photo_link(std::optional<std::string> x);

    user& set_me(std::optional<bool> x);

    user& set_permission_id(std::optional<std::string> x);

    user& set_email_address(std::optional<std::string> x);

private:
    ccd::var m_json;
};

using user_list_t = std::vector<user>;

}
}
}
