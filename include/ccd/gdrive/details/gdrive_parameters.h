
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/executor.h>

#include <optional>
#include <string>

namespace ccd::gdrive::details
{

template <typename T>
class with_common_parameters
{
public:
    /// @brief Selector specifying which fields to include in a partial response
    T& set_fields(std::optional<std::string> x)
    {
        m_fields = std::move(x);
        return static_cast<T&>(*this);
    }

    /// @brief Data format for the response
    T& set_alt(std::optional<std::string> x)
    {
        m_alt = std::move(x);
        return static_cast<T&>(*this);
    }

    /// @brief Returns response with indentations and line breaks
    T& set_pretty_print(std::optional<bool> x)
    {
        m_pretty_print = x;
        return static_cast<T&>(*this);
    }

    /// @brief An opaque string that represents a user for quota purposes
    T& set_quota_user(std::optional<std::string> x)
    {
        m_quota_user = std::move(x);
        return static_cast<T&>(*this);
    }

    /// @brief Deprecated
    T& set_user_ip(std::optional<std::string> x)
    {
        m_user_ip = std::move(x);
        return static_cast<T&>(*this);
    }

protected:
    void add_common_parameters(http_executor& e)
    {
        if (m_fields)
            e.append_query("fields", *m_fields);
        if (m_alt)
            e.append_query("alt", *m_alt);
        if (m_pretty_print)
            e.append_query("prettyPrint", *m_pretty_print ? "true" : "false");
        if (m_quota_user)
            e.append_query("quotaUser", *m_quota_user);
        if (m_user_ip)
            e.append_query("userIp", *m_user_ip);
    }

private:
    std::optional<std::string> m_fields;
    std::optional<std::string> m_alt;
    std::optional<bool> m_pretty_print;
    std::optional<std::string> m_quota_user;
    std::optional<std::string> m_user_ip;
};

template <typename T>
class with_file_parameters : public with_common_parameters<T>
{
public:
    /// @brief Whether the requesting application supports Team Drives
    T& set_support_team_drives(std::optional<bool> x)
    {
        m_support_team_drives = x;
        return static_cast<T&>(*this);
    }

protected:
    void add_file_parameters(http_executor& e)
    {
        this->add_common_parameters(e);
        if (m_support_team_drives)
            e.append_query("supportsTeamDrives", *m_support_team_drives ? "true" : "false");
    }

private:
    std::optional<bool> m_support_team_drives;
};

template <typename T>
class with_change_file_parameters : public with_file_parameters<T>
{
public:
    /// @brief Whether to set the 'keepForever' field in the new head revision
    T& set_keep_revision_forever(std::optional<bool> x)
    {
        m_keep_revision_forever = x;
        return static_cast<T&>(*this);
    }

    /// @brief A language hint for OCR processing during image import (ISO 639-1 code)
    T& set_ocr_language(std::optional<std::string> x)
    {
        m_ocr_language = std::move(x);
        return static_cast<T&>(*this);
    }

protected:
    void add_change_file_parameters(http_executor& e)
    {
        this->add_file_parameters(e);
        if (m_keep_revision_forever)
            e.append_query("keepRevisionForever", *m_keep_revision_forever ? "true" : "false");
        if (m_ocr_language)
            e.append_query("ocrLanguage", *m_ocr_language);
    }

private:
    std::optional<bool> m_keep_revision_forever;
    std::optional<std::string> m_ocr_language;
};

template <typename T>
class with_ignore_default_visibility_parameter
{
public:
    /// @brief Whether to ignore the domain's default visibility settings for the created file
    T& set_ignore_default_visibility(std::optional<bool> x)
    {
        m_ignore_default_visibility = x;
        return static_cast<T&>(*this);
    }

protected:
    void add_ignore_default_visibility_parameter(http_executor& e)
    {
        if (m_ignore_default_visibility)
            e.append_query("ignoreDefaultVisibility", *m_ignore_default_visibility ? "true" : "false");
    }

private:
    std::optional<bool> m_ignore_default_visibility;
};

template <typename T>
class with_use_content_as_indexable_text_parameter
{
public:
    /// @brief Whether to use the uploaded content as indexable text
    T& set_use_content_as_indexable_text(std::optional<bool> x)
    {
        m_use_content_as_indexable_text = x;
        return static_cast<T&>(*this);
    }

protected:
    void add_use_content_as_indexable_text_parameter(http_executor& e)
    {
        if (m_use_content_as_indexable_text)
            e.append_query("ignoreDefaultVisibility", *m_use_content_as_indexable_text ? "true" : "false");
    }

private:
    std::optional<bool> m_use_content_as_indexable_text;
};

}
