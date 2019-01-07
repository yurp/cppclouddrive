
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/defs.h>

#include <optional>

namespace ccd
{

/// @brief creates http_client for gdrive API.
///        With auth URI https://accounts.google.com/o/oauth2/auth,
///        token URI https://oauth2.googleapis.com/token,
///        redirect URI urn:ietf:wg:oauth:2.0:oob.
///        scope https://www.googleapis.com/auth/drive,
///        The client doesn't request responce compressing.
///        If the function is run for new user, or the token has been expired, then the function will print URL to cout,
///        which should be opened by user. Once user follows all instruction at the URL page and receives token, they
///        should enter this token to cin
/// @param[in] oauth2_client_id     OAuth2 client (application) ID
/// @param[in] oauth2_client_secret OAuth client's secret
/// @param[in] oauth2_token_file    json file for keeping OAuth2 token. If it doesn't exists, the function will create
///                                 it
pplx::task<http_client_ptr> create_gdrive_http_client(std::string oauth2_client_id,
                                                      std::string oauth2_client_secret,
                                                      std::string oauth2_token_file);

namespace details
{

std::optional<string_list_t> create_string_list(web::json::value& js, std::string key);

void string_list_to_json(web::json::value& js, std::string key, const std::optional<string_list_t>& lst);

std::optional<key_value_list_t> create_key_value_list(web::json::value& js, std::string key);

void key_value_list_to_json(web::json::value& js, std::string key, const std::optional<key_value_list_t>& lst);

std::optional<std::string> get_string(const web::json::value& js, std::string key);

void set_string(web::json::value& js, std::string key, std::optional<std::string> value);

std::optional<bool> get_bool(const web::json::value& js, std::string key);

void set_bool(web::json::value& js, std::string key, std::optional<bool> value);

std::optional<double> get_double(const web::json::value& js, std::string key);

std::optional<int64_t> get_int64(const web::json::value& js, std::string key);

std::optional<int32_t> get_int32(const web::json::value& js, std::string key);

template<typename T>
void set_number(web::json::value& js, std::string key, std::optional<T> value)
{
    auto k = utility::conversions::to_string_t(std::move(key));

    if (!js.is_object())
    {
        return;
    }

    if (value)
    {
        js[k] = web::json::value::number(*value);
    } else
    {
        js.erase(k);
    }
}

template<typename T>
std::optional<T> create_object(web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));
    if (js.is_object() && js.has_field(k) && js.at(k).is_object())
    {
        return T{ std::exchange(js.at(k), web::json::value{ }) };
    }

    return std::nullopt;
}

template<typename T>
void object_to_json(web::json::value& js, std::string key, const std::optional<T>& obj)
{
    if (obj)
    {
        auto k = utility::conversions::to_string_t(std::move(key));
        js[k] = obj->to_json();
    }
}

template<typename T>
std::optional<std::vector<T>> create_object_list(web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));
    if (js.is_object() && js.has_field(k) && js.at(k).is_array())
    {
        std::optional<std::vector<T>> lst = std::vector<T>{ };
        for (auto& i: js.at(k).as_array())
        {
            lst->emplace_back(std::exchange(i, web::json::value{ }));
        }

        return lst;
    }

    return std::nullopt;
}

template<typename T>
void object_list_to_json(web::json::value& js, std::string key, const std::optional<std::vector<T>>& lst)
{
    if (lst)
    {
        auto k = utility::conversions::to_string_t(std::move(key));
        js[k] = web::json::value::array();
        auto& js_arr = js[k];
        int j = 0;
        for (const auto& i: *lst)
        {
            js_arr[j++] = i.to_json();
        }
    }
}

}
}
