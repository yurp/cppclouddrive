
#include <ccd/dropbox/model/dropbox_metadata.h>
#include <ccd/utils.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace model
{

dimensions::dimensions()
    : m_json(web::json::value::object())
{
}

dimensions::dimensions(web::json::value js)
    : m_json(std::move(js))
{
}

web::json::value dimensions::to_json() const
{
    return m_json;
}

dimensions& dimensions::set_height(std::optional<int64_t> x)
{
    details::set_number(m_json, "height", x);
    return *this;
}

dimensions& dimensions::set_width(std::optional<int64_t> x)
{
    details::set_number(m_json, "width", x);
    return *this;
}

std::optional<int64_t> dimensions::get_height() const
{
    return details::get_int64(m_json, "height");
}

std::optional<int64_t> dimensions::get_width() const
{
    return details::get_int64(m_json, "width");
}

gps_coordinates::gps_coordinates()
    : m_json(web::json::value::object())
{
}

gps_coordinates::gps_coordinates(web::json::value js)
    : m_json(std::move(js))
{
}

web::json::value gps_coordinates::to_json() const
{
    return m_json;
}

gps_coordinates& gps_coordinates::set_latitude(std::optional<double> x)
{
    details::set_number(m_json, "latitude", x);
    return *this;
}

gps_coordinates& gps_coordinates::set_longitude(std::optional<double> x)
{
    details::set_number(m_json, "longitude", x);
    return *this;
}

std::optional<double> gps_coordinates::get_latitude() const
{
    return details::get_double(m_json, "latitude");
}

std::optional<double> gps_coordinates::get_longitude() const
{
    return details::get_double(m_json, "longitude");
}

media_info::media_info()
    : m_json(web::json::value::object())
{
}

media_info::media_info(web::json::value js)
    : m_json(std::move(js))
{
}

web::json::value media_info::to_json() const
{
    return m_json;
}

media_info& media_info::set_type(tag_type x)
{
    /// TODO: implement
    return *this;
}

media_info& media_info::set_dimensions(std::optional<dimensions> x)
{
    details::object_to_json(m_json[utility::conversions::to_string_t("metadata")], "dimensions", std::move(x));
    return *this;
}

media_info& media_info::set_location(std::optional<gps_coordinates> x)
{
    details::object_to_json(m_json[utility::conversions::to_string_t("metadata")], "location", std::move(x));
    return *this;
}

media_info& media_info::set_time_taken(std::optional<std::string> x)
{
    details::set_string(m_json[utility::conversions::to_string_t("metadata")], "time_taken", std::move(x));
    return *this;
}

media_info& media_info::set_duration(std::optional<int64_t> x)
{
    details::set_number(m_json[utility::conversions::to_string_t("metadata")], "duration", x);
    return *this;
}

media_info::tag_type media_info::get_type() const
{
    // TODO: implement
    return tag_type::unknown;
}

std::optional<dimensions> media_info::get_dimensions() const
{
    auto metadata_field = utility::conversions::to_string_t("metadata");
    if (m_json.is_object() && m_json.has_field(metadata_field))
    {
        return details::create_object_copy<dimensions>(m_json.at(metadata_field), "dimensions");
    }

    return std::nullopt;
}

std::optional<gps_coordinates> media_info::get_location() const
{
    auto metadata_field = utility::conversions::to_string_t("metadata");
    if (m_json.is_object() && m_json.has_field(metadata_field))
    {
        return details::create_object_copy<gps_coordinates>(m_json.at(metadata_field), "location");
    }

    return std::nullopt;
}

std::optional<std::string> media_info::get_time_taken() const
{
    auto metadata_field = utility::conversions::to_string_t("metadata");
    if (m_json.is_object() && m_json.has_field(metadata_field))
    {
        return details::get_string(m_json.at(metadata_field), "time_taken");
    }

    return std::nullopt;
}

std::optional<int64_t> media_info::get_duration() const
{
    auto metadata_field = utility::conversions::to_string_t("metadata");
    if (m_json.is_object() && m_json.has_field(metadata_field))
    {
        return details::get_int64(m_json.at(metadata_field), "duration");
    }

    return std::nullopt;
}

sharing_info::sharing_info()
    : m_json(web::json::value::object())
{
}

sharing_info::sharing_info(web::json::value js)
    : m_json(std::move(js))
{
}

web::json::value sharing_info::to_json() const
{
    return m_json;
}

sharing_info& sharing_info::set_read_only(std::optional<bool> x)
{
    details::set_bool(m_json, "read_only", x);
    return *this;
}

sharing_info& sharing_info::set_parent_shared_folder_id(std::optional<std::string> x)
{
    details::set_string(m_json, "parent_shared_folder_id", std::move(x));
    return *this;
}

sharing_info& sharing_info::set_modified_by(std::optional<std::string> x)
{
    details::set_string(m_json, "modified_by", std::move(x));
    return *this;
}

sharing_info& sharing_info::set_shared_folder_id(std::optional<std::string> x)
{
    details::set_string(m_json, "shared_folder_id", std::move(x));
    return *this;
}

sharing_info& sharing_info::set_traverse_only(std::optional<bool> x)
{
    details::set_bool(m_json, "traverse_only", x);
    return *this;
}

sharing_info& sharing_info::set_no_access(std::optional<bool> x)
{
    details::set_bool(m_json, "no_access", x);
    return *this;
}

std::optional<bool> sharing_info::get_read_only() const
{
    return details::get_bool(m_json, "read_only");
}

std::optional<std::string> sharing_info::get_parent_shared_folder_id() const
{
    return details::get_string(m_json, "parent_shared_folder_id");
}

std::optional<std::string> sharing_info::get_modified_by() const
{
    return details::get_string(m_json, "modified_by");
}

std::optional<std::string> sharing_info::get_shared_folder_id() const
{
    return details::get_string(m_json, "shared_folder_id");
}

std::optional<bool> sharing_info::get_traverse_only() const
{
    return details::get_bool(m_json, "traverse_only");
}

std::optional<bool> sharing_info::get_no_access() const
{
    return details::get_bool(m_json, "no_access");
}

property_field::property_field()
    : m_json(web::json::value::object())
{
}

property_field::property_field(web::json::value js)
    : m_json(std::move(js))
{
}

web::json::value property_field::to_json() const
{
    return m_json;
}

property_field& property_field::set_name(std::optional<std::string> x)
{
    details::set_string(m_json, "name", std::move(x));
    return *this;
}

property_field& property_field::set_value(std::optional<std::string> x)
{
    details::set_string(m_json, "value", std::move(x));
    return *this;
}

std::optional<std::string> property_field::get_name() const
{
    return details::get_string(m_json, "name");
}

std::optional<std::string> property_field::get_value() const
{
    return details::get_string(m_json, "value");
}

property_group::property_group()
    : m_json(web::json::value::object())
{
}

property_group::property_group(web::json::value js)
    : m_json(std::move(js))
{
}

web::json::value property_group::to_json() const
{
    return m_json;
}

property_group& property_group::set_template_id(std::optional<std::string> x)
{
    details::set_string(m_json, "template_id", std::move(x));
    return *this;
}

property_group& property_group::set_fields(std::optional<property_field_list_t> x)
{
    details::object_list_to_json(m_json, "fields", std::move(x));
    return *this;
}

std::optional<std::string> property_group::get_template_id() const
{
    return details::get_string(m_json, "template_id");
}

std::optional<property_field_list_t> property_group::get_fields() const
{
    return details::create_object_list_copy<property_field>(m_json, "fields");
}

metadata::metadata()
    : m_json(web::json::value::object())
{
}

metadata::metadata(web::json::value js)
    : m_json(std::move(js))
{
}

web::json::value metadata::to_json() const
{
    return m_json;
}

metadata& metadata::set_type(tag_type x)
{
    // TODO: implement
    return *this;
}

metadata& metadata::set_name(std::optional<std::string> x)
{
    details::set_string(m_json, "name", std::move(x));
    return *this;
}

metadata& metadata::set_id(std::optional<std::string> x)
{
    details::set_string(m_json, "id", std::move(x));
    return *this;
}

metadata& metadata::set_client_modified(std::optional<std::string> x)
{
    details::set_string(m_json, "client_modified", std::move(x));
    return *this;
}

metadata& metadata::set_server_modified(std::optional<std::string> x)
{
    details::set_string(m_json, "server_modified", std::move(x));
    return *this;
}

metadata& metadata::set_rev(std::optional<std::string> x)
{
    details::set_string(m_json, "rev", std::move(x));
    return *this;
}

metadata& metadata::set_size(std::optional<int64_t> x)
{
    details::set_number(m_json, "size", x);
    return *this;
}

metadata& metadata::set_path_lower(std::optional<std::string> x)
{
    details::set_string(m_json, "path_lower", std::move(x));
    return *this;
}

metadata& metadata::set_path_display(std::optional<std::string> x)
{
    details::set_string(m_json, "path_display", std::move(x));
    return *this;
}

metadata& metadata::set_parent_shared_folder_id(std::optional<std::string> x)
{
    details::set_string(m_json, "parent_shared_folder_id", std::move(x));
    return *this;
}

metadata& metadata::set_media_info(std::optional<media_info> x)
{
    details::object_to_json(m_json, "media_info", std::move(x));
    return *this;
}

metadata& metadata::set_symlink_info_target(std::optional<std::string> x)
{
    details::set_string(m_json[utility::conversions::to_string_t("symlink_info")], "target", std::move(x));
    return *this;
}

metadata& metadata::set_sharing_info(std::optional<sharing_info> x)
{
    details::object_to_json(m_json, "sharing_info", std::move(x));
    return *this;
}

metadata& metadata::set_property_groups(std::optional<property_group_list_t> x)
{
    details::object_list_to_json(m_json, "property_groups", std::move(x));
    return *this;
}

metadata& metadata::set_has_explicit_shared_members(std::optional<bool> x)
{
    details::set_bool(m_json, "has_explicit_shared_members", x);
    return *this;
}

metadata& metadata::set_content_hash(std::optional<std::string> x)
{
    details::set_string(m_json, "content_hash", std::move(x));
    return *this;
}

metadata& metadata::set_shared_folder_id(std::optional<std::string> x)
{
    details::set_string(m_json, "shared_folder_id", std::move(x));
    return *this;
}

metadata::tag_type metadata::get_tag() const
{
    // TODO: implement
    return tag_type::unknown;
}

std::optional<std::string> metadata::get_name() const
{
    return details::get_string(m_json, "name");
}

std::optional<std::string> metadata::get_id() const
{
    return details::get_string(m_json, "id");
}

std::optional<std::string> metadata::get_client_modified() const
{
    return details::get_string(m_json, "client_modified");
}

std::optional<std::string> metadata::get_server_modified() const
{
    return details::get_string(m_json, "server_modified");
}

std::optional<std::string> metadata::get_rev() const
{
    return details::get_string(m_json, "rev");
}

std::optional<int64_t> metadata::get_size() const
{
    return details::get_int64(m_json, "size");
}

std::optional<std::string> metadata::get_path_lower() const
{
    return details::get_string(m_json, "path_lower");
}

std::optional<std::string> metadata::get_path_display() const
{
    return details::get_string(m_json, "path_display");
}

std::optional<std::string> metadata::get_parent_shared_folder_id() const
{
    return details::get_string(m_json, "parent_shared_folder_id");
}

std::optional<media_info> metadata::get_media_info() const
{
    return details::create_object_copy<media_info>(m_json, "media_info");
}

std::optional<std::string> metadata::get_symlink_info_target() const
{
    auto symlink_info_field = utility::conversions::to_string_t("symlink_info");
    if (m_json.is_object() && m_json.has_field(symlink_info_field))
    {
        return details::get_string(m_json.at(symlink_info_field), "target");
    }

    return std::nullopt;
}

std::optional<sharing_info> metadata::get_sharing_info() const
{
    return details::create_object_copy<sharing_info>(m_json, "sharing_info");
}

std::optional<property_group_list_t> metadata::get_property_groups() const
{
    return details::create_object_list_copy<property_group>(m_json, "property_groups");
}

std::optional<bool> metadata::get_has_explicit_shared_members() const
{
    return details::get_bool(m_json, "has_explicit_shared_members");
}

std::optional<std::string> metadata::get_content_hash() const
{
    return details::get_string(m_json, "content_hash");
}

std::optional<std::string> metadata::get_shared_folder_id() const
{
    return details::get_string(m_json, "shared_folder_id");
}

metadata_list::metadata_list()
    : m_json(web::json::value::object())
{
}

metadata_list::metadata_list(web::json::value js)
    : m_json(std::move(js))
{
}

web::json::value metadata_list::to_json() const
{
    return m_json;
}

metadata_list& metadata_list::set_entries(std::optional<metadata_list_t> x)
{
    details::object_list_to_json(m_json, "entries", std::move(x));
    return *this;
}

metadata_list& metadata_list::set_cursor(std::optional<std::string> x)
{
    details::set_string(m_json, "cursor", std::move(x));
    return *this;
}

metadata_list& metadata_list::set_has_more(std::optional<bool> x)
{
    details::set_bool(m_json, "has_more", x);
    return *this;
}

std::optional<metadata_list_t> metadata_list::get_entries() const
{
    return details::create_object_list_copy<metadata>(m_json, "entries");
}

std::optional<std::string> metadata_list::get_cursor() const
{
    return details::get_string(m_json, "cursor");
}

std::optional<bool> metadata_list::get_has_more() const
{
    return details::get_bool(m_json, "has_more");
}

}
}
}
