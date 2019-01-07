
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/defs.h>
#include <ccd/gdrive/model/gdrive_permission.h>
#include <ccd/gdrive/model/gdrive_user.h>

#include <cpprest/json.h>

#include <optional>

namespace ccd::gdrive
{
inline namespace v3
{
namespace model
{

class file
{
public:
    class capabilities
    {
    public:
        capabilities();

        explicit capabilities(web::json::value js);

        web::json::value to_json() const;

        std::optional<bool> get_can_add_children() const;

        std::optional<bool> get_can_change_copy_requires_writer_permission() const;

        std::optional<bool> get_can_change_viewers_can_copy_content() const;

        std::optional<bool> get_can_comment() const;

        std::optional<bool> get_can_copy() const;

        std::optional<bool> get_can_delete() const;

        std::optional<bool> get_can_delete_children() const;

        std::optional<bool> get_can_download() const;

        std::optional<bool> get_can_edit() const;

        std::optional<bool> get_can_list_children() const;

        std::optional<bool> get_can_move_children_out_of_team_drive() const;

        std::optional<bool> get_can_move_children_within_team_drive() const;

        std::optional<bool> get_can_move_item_into_team_drive() const;

        std::optional<bool> get_can_move_item_out_of_team_drive() const;

        std::optional<bool> get_can_move_item_within_team_drive() const;

        std::optional<bool> get_can_move_team_drive_item() const;

        std::optional<bool> get_can_read_revisions() const;

        std::optional<bool> get_can_read_team_drive() const;

        std::optional<bool> get_can_remove_children() const;

        std::optional<bool> get_can_rename() const;

        std::optional<bool> get_can_share() const;

        std::optional<bool> get_can_trash() const;

        std::optional<bool> get_can_trash_children() const;

        std::optional<bool> get_can_untrash() const;

        capabilities& set_can_add_children(std::optional<bool> x);

        capabilities& set_can_change_copy_requires_writer_permission(std::optional<bool> x);

        capabilities& set_can_change_viewers_can_copy_content(std::optional<bool> x);

        capabilities& set_can_comment(std::optional<bool> x);

        capabilities& set_can_copy(std::optional<bool> x);

        capabilities& set_can_delete(std::optional<bool> x);

        capabilities& set_can_delete_children(std::optional<bool> x);

        capabilities& set_can_download(std::optional<bool> x);

        capabilities& set_can_edit(std::optional<bool> x);

        capabilities& set_can_list_children(std::optional<bool> x);

        capabilities& set_can_move_children_out_of_team_drive(std::optional<bool> x);

        capabilities& set_can_move_children_within_team_drive(std::optional<bool> x);

        capabilities& set_can_move_item_into_team_drive(std::optional<bool> x);

        capabilities& set_can_move_item_out_of_team_drive(std::optional<bool> x);

        capabilities& set_can_move_item_within_team_drive(std::optional<bool> x);

        capabilities& set_can_move_team_drive_item(std::optional<bool> x);

        capabilities& set_can_read_revisions(std::optional<bool> x);

        capabilities& set_can_read_team_drive(std::optional<bool> x);

        capabilities& set_can_remove_children(std::optional<bool> x);

        capabilities& set_can_rename(std::optional<bool> x);

        capabilities& set_can_share(std::optional<bool> x);

        capabilities& set_can_trash(std::optional<bool> x);

        capabilities& set_can_trash_children(std::optional<bool> x);

        capabilities& set_can_untrash(std::optional<bool> x);

    private:
        web::json::value m_json;
    };

    class content_hints
    {
    public:
        class thumbnail
        {
        public:
            thumbnail();

            explicit thumbnail(web::json::value js);

            web::json::value to_json() const;

            /// @brief The thumbnail data encoded with URL-safe Base64 (RFC 4648 section 5)
            std::optional<std::string> get_image() const;

            std::optional<std::string> get_mime_type() const;

            /// @brief The thumbnail data encoded with URL-safe Base64 (RFC 4648 section 5)
            thumbnail& set_image(std::optional<std::string> x);

            thumbnail& set_mime_type(std::optional<std::string> x);

        private:
            web::json::value m_json;
        };

        content_hints();

        explicit content_hints(web::json::value js);

        web::json::value to_json() const;

        std::optional<thumbnail> get_thumbnail() const;

        std::optional<std::string> get_indexable_text() const;

        content_hints& set_thumbnail(std::optional<thumbnail> x);

        content_hints& set_indexable_text(std::optional<std::string> x);

    private:
        web::json::value m_json;
        std::optional<thumbnail> m_thumbnail;
    };

    class image_media_metadata
    {
    public:
        class location
        {
        public:
            location();

            explicit location(web::json::value js);

            web::json::value to_json() const;

            std::optional<double> get_latitude() const;

            std::optional<double> get_longitude() const;

            std::optional<double> get_altitude() const;

            location& set_latitude(std::optional<double> x);

            location& set_longitude(std::optional<double> x);

            location& set_altitude(std::optional<double> x);

        private:
            web::json::value m_json;
        };

        image_media_metadata();

        explicit image_media_metadata(web::json::value js);

        web::json::value to_json() const;

        std::optional<int32_t> get_width() const;

        std::optional<int32_t> get_height() const;

        std::optional<int32_t> get_rotation() const;

        std::optional<location> get_location() const;

        std::optional<std::string> get_time() const;

        std::optional<std::string> get_camera_make() const;

        std::optional<std::string> get_camera_model() const;

        std::optional<double> get_exposure_time() const;

        std::optional<double> get_aperture() const;

        std::optional<bool> get_flash_used() const;

        std::optional<double> get_focal_length() const;

        std::optional<int32_t> get_iso_speed() const;

        std::optional<std::string> get_metering_mode() const;

        std::optional<std::string> get_sensor() const;

        std::optional<std::string> get_exposure_mode() const;

        std::optional<std::string> get_color_space() const;

        std::optional<std::string> get_white_balance() const;

        std::optional<double> get_exposure_bias() const;

        std::optional<double> get_max_aperture_value() const;

        std::optional<int32_t> get_subject_distance() const;

        std::optional<std::string> get_lens() const;

        image_media_metadata& set_width(std::optional<int32_t> x);

        image_media_metadata& set_height(std::optional<int32_t> x);

        image_media_metadata& set_rotation(std::optional<int32_t> x);

        image_media_metadata& set_location(std::optional<location> x);

        image_media_metadata& set_time(std::optional<std::string> x);

        image_media_metadata& set_camera_make(std::optional<std::string> x);

        image_media_metadata& set_camera_model(std::optional<std::string> x);

        image_media_metadata& set_exposure_time(std::optional<double> x);

        image_media_metadata& set_aperture(std::optional<double> x);

        image_media_metadata& set_flash_used(std::optional<bool> x);

        image_media_metadata& set_focal_length(std::optional<double> x);

        image_media_metadata& set_iso_lpeed(std::optional<int32_t> x);

        image_media_metadata& set_metering_mode(std::optional<std::string> x);

        image_media_metadata& set_sensor(std::optional<std::string> x);

        image_media_metadata& set_exposure_mode(std::optional<std::string> x);

        image_media_metadata& set_color_space(std::optional<std::string> x);

        image_media_metadata& set_white_balance(std::optional<std::string> x);

        image_media_metadata& set_exposure_bias(std::optional<double> x);

        image_media_metadata& set_max_aperture_value(std::optional<double> x);

        image_media_metadata& set_subject_distance(std::optional<int32_t> x);

        image_media_metadata& set_lens(std::optional<std::string> x);

    private:
        web::json::value m_json;
        std::optional<location> m_location;
    };

    class video_media_metadata
    {
    public:
        video_media_metadata();

        explicit video_media_metadata(web::json::value js);

        web::json::value to_json() const;

        std::optional<int32_t> get_width() const;

        std::optional<int32_t> get_heigth() const;

        std::optional<int64_t> get_duration_millis() const;

        video_media_metadata& get_width(std::optional<int32_t> x);

        video_media_metadata& get_heigth(std::optional<int32_t> x);

        video_media_metadata& get_duration_millis(std::optional<int64_t> x);

    private:
        web::json::value m_json;
    };

    file();

    explicit file(web::json::value js);

    web::json::value to_json() const;

    std::optional<std::string> get_id() const;

    std::optional<std::string> get_name() const;

    std::optional<std::string> get_mime_type() const;

    std::optional<std::string> get_description() const;

    std::optional<bool> get_starred() const;

    std::optional<bool> get_trashed() const;

    std::optional<bool> get_explicitly_trashed() const;

    std::optional<user> get_trashing_user() const;

    std::optional<timestamp_t> get_trashed_time() const;

    std::optional<string_list_t> get_parents() const;

    std::optional<key_value_list_t> get_properties() const;

    std::optional<key_value_list_t> get_app_properties() const;

    std::optional<string_list_t> get_spaces() const;

    std::optional<int64_t> get_version() const;

    std::optional<std::string> get_web_content_link() const;

    std::optional<std::string> get_web_view_link() const;

    std::optional<std::string> get_icon_link() const;

    std::optional<bool> get_has_thumbnail() const;

    std::optional<std::string> get_thumbnail_link() const;

    std::optional<int64_t> get_thumbnail_version() const;

    std::optional<bool> get_viewed_by_me() const;

    std::optional<timestamp_t> get_viewed_by_me_time() const;

    std::optional<timestamp_t> get_created_time() const;

    std::optional<timestamp_t> get_modified_time() const;

    std::optional<timestamp_t> get_modified_by_me_time() const;

    std::optional<bool> get_modified_by_me() const;

    std::optional<timestamp_t> get_shared_with_me_time() const;

    std::optional<user> get_sharing_user() const;

    std::optional<std::vector<user>> get_owners() const;

    std::optional<std::string> get_teamdrive_id() const;

    std::optional<user> get_last_modifying_user() const;

    std::optional<bool> get_shared() const;

    std::optional<bool> get_owned_by_me() const;

    std::optional<capabilities> get_capabilities() const;

    std::optional<bool> get_viewers_can_copy_content() const;

    std::optional<bool> get_copy_requires_writer_permission() const;

    std::optional<bool> get_writers_can_share() const;

    std::optional<permission_list_t> get_permissions() const;

    std::optional<string_list_t> get_permission_ids() const;

    std::optional<bool> get_has_augmented_permissions() const;

    std::optional<std::string> get_folder_color_rgb() const;

    std::optional<std::string> get_original_filename() const;

    std::optional<std::string> get_full_file_extension() const;

    std::optional<std::string> get_file_extension() const;

    std::optional<std::string> get_md5_checksum() const;

    std::optional<int64_t> get_size() const;

    std::optional<int64_t> get_quota_bytes_used() const;

    std::optional<std::string> get_head_revision_id() const;

    std::optional<content_hints> get_content_hints() const;

    std::optional<image_media_metadata> get_image_media_metadata() const;

    std::optional<video_media_metadata> get_video_media_metadata() const;

    std::optional<bool> get_is_app_authorized() const;

    std::optional<key_value_list_t> get_export_links() const;

    file& set_id(std::optional<std::string> x);

    file& set_name(std::optional<std::string> x);

    file& set_mime_type(std::optional<std::string> x);

    file& set_description(std::optional<std::string> x);

    file& set_starred(std::optional<bool> x);

    file& set_trashed(std::optional<bool> x);

    file& set_explicitly_trashed(std::optional<bool> x);

    file& set_trashing_user(std::optional<user> x);

    file& set_trashed_time(std::optional<timestamp_t> x);

    file& set_parents(std::string x);

    file& set_parents(string_list_t x);

    file& set_parents(std::optional<string_list_t> x);

    file& set_properties(std::optional<key_value_list_t> x);

    file& set_app_properties(std::optional<key_value_list_t> x);

    file& set_spaces(std::optional<string_list_t> x);

    file& set_version(std::optional<int64_t> x);

    file& set_web_content_link(std::optional<std::string> x);

    file& set_web_view_link(std::optional<std::string> x);

    file& set_icon_link(std::optional<std::string> x);

    file& set_has_thumbnail(std::optional<bool> x);

    file& set_thumbnail_link(std::optional<std::string> x);

    file& set_thumbnail_version(std::optional<int64_t> x);

    file& set_viewed_by_me(std::optional<bool> x);

    file& set_viewed_by_me_time(std::optional<timestamp_t> x);

    file& set_created_time(std::optional<timestamp_t> x);

    file& set_modified_time(std::optional<timestamp_t> x);

    file& set_modified_by_me_time(std::optional<timestamp_t> x);

    file& set_modified_by_me(std::optional<bool> x);

    file& set_shared_with_me_time(std::optional<timestamp_t> x);

    file& set_sharing_user(std::optional<user> x);

    file& set_owners(std::optional<std::vector<user>> x);

    file& set_teamdrive_id(std::optional<std::string> x);

    file& set_last_modifying_user(std::optional<user> x);

    file& set_shared(std::optional<bool> x);

    file& set_owned_by_me(std::optional<bool> x);

    file& set_capabilities(std::optional<capabilities> x);

    file& set_viewers_can_copy_content(std::optional<bool> x);

    file& set_copy_requires_writer_permission(std::optional<bool> x);

    file& set_writers_can_share(std::optional<bool> x);

    file& set_permissions(std::optional<permission_list_t> x);

    file& set_permission_ids(std::optional<string_list_t> x);

    file& set_has_augmented_permissions(std::optional<bool> x);

    file& set_folder_color_rgb(std::optional<std::string> x);

    file& set_original_filename(std::optional<std::string> x);

    file& set_full_file_extension(std::optional<std::string> x);

    file& set_file_extension(std::optional<std::string> x);

    file& set_md5_checksum(std::optional<std::string> x);

    file& set_size(std::optional<int64_t> x);

    file& set_quota_bytes_used(std::optional<int64_t> x);

    file& set_head_revision_id(std::optional<std::string> x);

    file& set_content_hints(std::optional<content_hints> x);

    file& set_image_media_metadata(std::optional<image_media_metadata> x);

    file& set_video_media_metadata(std::optional<video_media_metadata> x);

    file& set_is_app_authorized(std::optional<bool> x);

    file& set_export_links(std::optional<key_value_list_t> x);

private:
    web::json::value m_json;
    std::optional<user> m_trashing_user;
    std::optional<string_list_t> m_parents;
    std::optional<key_value_list_t> m_properties;
    std::optional<key_value_list_t> m_app_properties;
    std::optional<string_list_t> m_spaces;
    std::optional<user> m_sharing_user;
    std::optional<user_list_t> m_owners;
    std::optional<user> m_last_modifying_user;
    std::optional<capabilities> m_capabilities;
    std::optional<permission_list_t> m_permissions;
    std::optional<string_list_t> m_permission_ids;
    std::optional<content_hints> m_content_hints;
    std::optional<image_media_metadata> m_image_media_metadata;
    std::optional<video_media_metadata> m_video_media_metadata;
    std::optional<key_value_list_t> m_export_links;
};

using file_list_t = std::vector<file>;

class file_list
{
public:
    file_list();

    explicit file_list(web::json::value js);

    web::json::value to_json() const;

    std::optional<std::string> get_next_page_token() const;

    std::optional<bool> get_incomplete_search() const;

    std::optional<file_list_t> get_files() const;

    file_list& set_next_page_token(std::optional<std::string> x);

    file_list& set_incomplete_search(std::optional<bool> x);

    file_list& set_files(std::optional<file_list_t> x);

private:
    web::json::value m_json;
    std::optional<file_list_t> m_files;
};

}
}
}
