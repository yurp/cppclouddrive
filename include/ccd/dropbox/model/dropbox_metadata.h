
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/defs.h>

#include <cpprest/json.h>

#include <optional>

namespace ccd::dropbox
{
inline namespace v2
{
namespace model
{

/// @brief Dimensions for a photo or video
class dimensions
{
public:
    dimensions();

    explicit dimensions(web::json::value js);

    web::json::value to_json() const;

    /// @brief Height of the photo/video.
    dimensions& set_height(std::optional<int64_t> x);

    /// @brief Width of the photo/video
    dimensions& set_width(std::optional<int64_t> x);

    /// @brief Height of the photo/video.
    std::optional<int64_t> get_height() const;

    /// @brief Width of the photo/video
    std::optional<int64_t> get_width() const;

private:
    web::json::value m_json;
};

class gps_coordinates
{
public:
    gps_coordinates();

    explicit gps_coordinates(web::json::value js);

    web::json::value to_json() const;

    /// @brief Latitude of the GPS coordinates.
    gps_coordinates& set_latitude(std::optional<double> x);

    /// @brief Longitude of the GPS coordinates
    gps_coordinates& set_longitude(std::optional<double> x);

    /// @brief Latitude of the GPS coordinates.
    std::optional<double> get_latitude() const;

    /// @brief Longitude of the GPS coordinates
    std::optional<double> get_longitude() const;

private:
    web::json::value m_json;
};

/// @brief The value will be one of the following datatypes: pending (void), photo, video
class media_info
{
public:
    media_info();

    explicit media_info(web::json::value js);

    web::json::value to_json() const;

    enum class tag_type { pending, photo, video, unknown };

    media_info& set_type(tag_type x);

    /// @brief Dimension of the photo/video. This field is optional.
    media_info& set_dimensions(std::optional<dimensions> x);

    /// @brief GpsCoordinates? The GPS coordinate of the photo/video. This field is optional.
    media_info& set_location(std::optional<gps_coordinates> x);

    /// @brief (format="%Y-%m-%dT%H:%M:%SZ")? The timestamp when the photo/video is taken. This field is optional.
    media_info& set_time_taken(std::optional<std::string> x);

    /// @brief The duration of the video in milliseconds. This field is optional.
    media_info& set_duration(std::optional<int64_t> x);

    tag_type get_type() const;

    /// @brief Dimension of the photo/video. This field is optional.
    std::optional<dimensions> get_dimensions() const;

    /// @brief GpsCoordinates? The GPS coordinate of the photo/video. This field is optional.
    std::optional<gps_coordinates> get_location() const;

    /// @brief (format="%Y-%m-%dT%H:%M:%SZ")? The timestamp when the photo/video is taken. This field is optional.
    std::optional<std::string> get_time_taken() const;

    /// @brief The duration of the video in milliseconds. This field is optional.
    std::optional<int64_t> get_duration() const;

private:
    web::json::value m_json;
};

/// @brief Sharing info for a file which is contained by a shared folder.
class sharing_info
{
public:
    sharing_info();

    explicit sharing_info(web::json::value js);

    web::json::value to_json() const;

    // file/folder
    /// @brief True if the file or folder is inside a read-only shared folder.
    sharing_info& set_read_only(std::optional<bool> x);

    // file/folder
    /// @brief (pattern="[-_0-9a-zA-Z:]+") ID of shared folder that holds this file.
    sharing_info& set_parent_shared_folder_id(std::optional<std::string> x);

    // file
    /// @brief (min_length=40, max_length=40)? The last user who modified the file. This field will be null if the
    ///        user's account has been deleted. This field is optional.
    sharing_info& set_modified_by(std::optional<std::string> x);

    // folder
    /// @brief (pattern="[-_0-9a-zA-Z:]+")? If this folder is a shared folder mount point, the ID of the shared folder
    ///        mounted at this location. This field is optional.
    sharing_info& set_shared_folder_id(std::optional<std::string> x);

    // folder
    /// @brief Specifies that the folder can only be traversed and the user can only see a limited subset of the
    ///        contents of this folder because they don't have read access to this folder. They do, however, have
    ///        access to some sub folder. The default for this field is False.
    sharing_info& set_traverse_only(std::optional<bool> x);

    // folder
    /// @brief Specifies that the folder cannot be accessed by the user. The default for this field is False.
    sharing_info& set_no_access(std::optional<bool> x);

    // file/folder
    /// @brief True if the file or folder is inside a read-only shared folder.
    std::optional<bool> get_read_only() const;

    // file/folder
    /// @brief (pattern="[-_0-9a-zA-Z:]+") ID of shared folder that holds this file.
    std::optional<std::string> get_parent_shared_folder_id() const;

    // file
    /// @brief (min_length=40, max_length=40)? The last user who modified the file. This field will be null if the
    ///        user's account has been deleted. This field is optional.
    std::optional<std::string> get_modified_by() const;

    // folder
    /// @brief (pattern="[-_0-9a-zA-Z:]+")? If this folder is a shared folder mount point, the ID of the shared folder
    ///        mounted at this location. This field is optional.
    std::optional<std::string> get_shared_folder_id() const;

    // folder
    /// @brief Specifies that the folder can only be traversed and the user can only see a limited subset of the
    ///        contents of this folder because they don't have read access to this folder. They do, however, have
    ///        access to some sub folder. The default for this field is False.
    std::optional<bool> get_traverse_only() const;

    // folder
    /// @brief Specifies that the folder cannot be accessed by the user. The default for this field is False.
    std::optional<bool> get_no_access() const;

private:
    web::json::value m_json;
};

/// @brief Raw key/value data to be associated with a Dropbox file. Property fields are added to Dropbox files as a
///        PropertyGroup. This datatype comes from an imported namespace originally defined in the file_properties
///        namespace.
class property_field
{
public:
    property_field();

    explicit property_field(web::json::value js);

    web::json::value to_json() const;

    /// @brief Key of the property field associated with a file and template. Keys can be up to 256 bytes.
    property_field& set_name(std::optional<std::string> x);

    /// @brief Value of the property field associated with a file and template. Values can be up to 1024 bytes.
    property_field& set_value(std::optional<std::string> x);

    /// @brief Key of the property field associated with a file and template. Keys can be up to 256 bytes.
    std::optional<std::string> get_name() const;

    /// @brief Value of the property field associated with a file and template. Values can be up to 1024 bytes.
    std::optional<std::string> get_value() const;

private:
    web::json::value m_json;
};

using property_field_list_t = std::vector<property_field>;

/// @brief A subset of the property fields described by the corresponding PropertyGroupTemplate. Properties are always
///        added to a Dropbox file as a PropertyGroup. The possible key names and value types in this group are defined
///        by the corresponding PropertyGroupTemplate. This datatype comes from an imported namespace originally
///        defined in the file_properties namespace.
class property_group
{
public:
    property_group();

    explicit property_group(web::json::value js);

    web::json::value to_json() const;

    /// @brief (min_length=1, pattern="(/|ptid:).*") A unique identifier for the associated template.
    property_group& set_template_id(std::optional<std::string> x);

    /// @brief The actual properties associated with the template. There can be up to 32 property types per template.
    property_group& set_fields(std::optional<property_field_list_t> x);

    /// @brief (min_length=1, pattern="(/|ptid:).*") A unique identifier for the associated template.
    std::optional<std::string> get_template_id() const;

    /// @brief The actual properties associated with the template. There can be up to 32 property types per template.
    std::optional<property_field_list_t> get_fields() const;

private:
    web::json::value m_json;
};

using property_group_list_t = std::vector<property_group>;

/// @brief Metadata for a file or folder. This datatype will be one of the following subtypes: file, folder, deleted
class metadata
{
public:
    metadata();

    explicit metadata(web::json::value js);

    web::json::value to_json() const;

    enum class tag_type { file, folder, deleted, unknown };

    metadata& set_type(tag_type x);

    // file/folder/deleted
    /// @brief The last component of the path (including extension). This never contains a slash
    metadata& set_name(std::optional<std::string> x);

     // file/folder
     /// @brief A unique identifier for the file
    metadata& set_id(std::optional<std::string> x);

     // file
     /// @brief Timestamp(format="%Y-%m-%dT%H:%M:%SZ") For files, this is the modification time set by the desktop
     ///        client when the file was added to Dropbox. Since this time is not verified (the Dropbox server stores
     ///        whatever the desktop client sends up), this should only be used for display purposes (such as sorting)
     ///        and not, for example, to determine if a file has changed or not
    metadata& set_client_modified(std::optional<std::string> x);

     // file
     /// @brief Timestamp(format="%Y-%m-%dT%H:%M:%SZ") The last time the file was modified on Dropbox
    metadata& set_server_modified(std::optional<std::string> x);

     // file
     /// @brief (min_length=9, pattern="[0-9a-f]+") A unique identifier for the current revision of a file. This field
     ///        is the same rev as elsewhere in the API and can be used to detect changes and avoid conflicts
    metadata& set_rev(std::optional<std::string> x);

     // file
     /// @brief The file size in bytes
    metadata& set_size(std::optional<int64_t> x);

     // file/folder/deleted
     /// @brief The lowercased full path in the user's Dropbox. This always starts with a slash. This field will be null
     ///        if the file or folder is not mounted. This field is optional.
    metadata& set_path_lower(std::optional<std::string> x);

     // file/folder/deleted
     /// @brief The cased path to be used for display purposes only. In rare instances the casing will not correctly
     ///        match the user's filesystem, but this behavior will match the path provided in the Core API v1, and at
     ///        least the last path component will have the correct casing. Changes to only the casing of paths won't
     ///        be returned by list_folder/continue. This field will be null if the file or folder is not mounted.
     ///        This field is optional.
    metadata& set_path_display(std::optional<std::string> x);

     // file/folder/deleted
     /// @brief (pattern="[-_0-9a-zA-Z:]+")? Field is deprecated. Please use FileSharingInfo.parent_shared_folder_id or
     ///        FolderSharingInfo.parent_shared_folder_id instead. This field is optional
    metadata& set_parent_shared_folder_id(std::optional<std::string> x);

     // file
     /// @brief Additional information if the file is a photo or video. This field is optional
    metadata& set_media_info(std::optional<media_info> x);

     // file
     /// @brief The target this symlink points to7 Set if this file is a symlink. This field is optional
    metadata& set_symlink_info_target(std::optional<std::string> x);

     // file/folder
     /// @brief Set if this file is contained in a shared folder. This field is optional.
    metadata& set_sharing_info(std::optional<sharing_info> x);

     // file/folder
     /// @brief Additional information if the file has custom properties with the property template specified.
     ///        This field is optional.
    metadata& set_property_groups(std::optional<property_group_list_t> x);

     // file
     /// @brief This flag will only be present if include_has_explicit_shared_members is true in list_folder or
     ///        get_metadata. If this flag is present, it will be true if this file has any explicit shared members.
     ///        This is different from sharing_info in that this could be true in the case where a file has explicit
     ///        members but is not contained within a shared folder. This field is optional.
    metadata& set_has_explicit_shared_members(std::optional<bool> x);

     // file
     /// @brief (min_length=64, max_length=64) A hash of the file content. This field can be used to verify data
     ///        integrity. For more information see our Content hash page. This field is optional.
    metadata& set_content_hash(std::optional<std::string> x);

     // folder
     /// @brief (pattern="[-_0-9a-zA-Z:]+") Field is deprecated. Please use sharing_info instead. This field is optional
    metadata& set_shared_folder_id(std::optional<std::string> x);

    tag_type get_tag() const;

    // file/folder/deleted
    /// @brief The last component of the path (including extension). This never contains a slash
    std::optional<std::string> get_name() const;

    // file/folder
    /// @brief A unique identifier for the file
    std::optional<std::string> get_id() const;

    // file
    /// @brief Timestamp(format="%Y-%m-%dT%H:%M:%SZ") For files, this is the modification time set by the desktop
    ///        client when the file was added to Dropbox. Since this time is not verified (the Dropbox server stores
    ///        whatever the desktop client sends up), this should only be used for display purposes (such as sorting)
    ///        and not, for example, to determine if a file has changed or not
    std::optional<std::string> get_client_modified() const;

    // file
    /// @brief Timestamp(format="%Y-%m-%dT%H:%M:%SZ") The last time the file was modified on Dropbox
    std::optional<std::string> get_server_modified() const;

    // file
    /// @brief (min_length=9, pattern="[0-9a-f]+") A unique identifier for the current revision of a file. This field
    ///        is the same rev as elsewhere in the API and can be used to detect changes and avoid conflicts
    std::optional<std::string> get_rev() const;

    // file
    /// @brief The file size in bytes
    std::optional<int64_t> get_size() const;

    // file/folder/deleted
    /// @brief The lowercased full path in the user's Dropbox. This always starts with a slash. This field will be null
    ///        if the file or folder is not mounted. This field is optional.
    std::optional<std::string> get_path_lower() const;

    // file/folder/deleted
    /// @brief The cased path to be used for display purposes only. In rare instances the casing will not correctly
    ///        match the user's filesystem, but this behavior will match the path provided in the Core API v1, and at
    ///        least the last path component will have the correct casing. Changes to only the casing of paths won't
    ///        be returned by list_folder/continue. This field will be null if the file or folder is not mounted.
    ///        This field is optional.
    std::optional<std::string> get_path_display() const;

    // file/folder/deleted
    /// @brief (pattern="[-_0-9a-zA-Z:]+")? Field is deprecated. Please use FileSharingInfo.parent_shared_folder_id or
    ///        FolderSharingInfo.parent_shared_folder_id instead. This field is optional
    std::optional<std::string> get_parent_shared_folder_id() const;

    // file
    /// @brief Additional information if the file is a photo or video. This field is optional
    std::optional<media_info> get_media_info() const;

    // file
    /// @brief The target this symlink points to7 Set if this file is a symlink. This field is optional
    std::optional<std::string> get_symlink_info_target() const;

    // file/folder
    /// @brief Set if this file is contained in a shared folder. This field is optional.
    std::optional<sharing_info> get_sharing_info() const;

    // file/folder
    /// @brief Additional information if the file has custom properties with the property template specified.
    ///        This field is optional.
    std::optional<property_group_list_t> get_property_groups() const;

    // file
    /// @brief This flag will only be present if include_has_explicit_shared_members is true in list_folder or
    ///        get_metadata. If this flag is present, it will be true if this file has any explicit shared members.
    ///        This is different from sharing_info in that this could be true in the case where a file has explicit
    ///        members but is not contained within a shared folder. This field is optional.
    std::optional<bool> get_has_explicit_shared_members() const;

    // file
    /// @brief (min_length=64, max_length=64) A hash of the file content. This field can be used to verify data
    ///        integrity. For more information see our Content hash page. This field is optional.
    std::optional<std::string> get_content_hash() const;

    // folder
    /// @brief (pattern="[-_0-9a-zA-Z:]+") Field is deprecated. Please use sharing_info instead. This field is optional
    std::optional<std::string> get_shared_folder_id() const;

private:
    web::json::value m_json;
};

using metadata_list_t = std::vector<metadata>;

class metadata_list
{
public:
    metadata_list();

    explicit metadata_list(web::json::value js);

    web::json::value to_json() const;

    /// @brief The files and (direct) subfolders in the folder.
    metadata_list& set_entries(std::optional<metadata_list_t> x);

    /// @brief Pass the cursor into list_folder/continue to see what's changed in the folder since your previous query.
    metadata_list& set_cursor(std::optional<std::string> x);

    /// @brief If true, then there are more entries available. Pass the cursor to list_folder_continue to retrieve the
    ///        rest.
    metadata_list& set_has_more(std::optional<bool> x);

    /// @brief The files and (direct) subfolders in the folder.
    std::optional<metadata_list_t> get_entries() const;

    /// @brief Pass the cursor into list_folder/continue to see what's changed in the folder since your previous query.
    std::optional<std::string> get_cursor() const;

    /// @brief If true, then there are more entries available. Pass the cursor to list_folder_continue to retrieve the
    ///        rest.
    std::optional<bool> get_has_more() const;

private:
    web::json::value m_json;
};

}
}
}
