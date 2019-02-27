
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/defs.h>

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

namespace ccd
{

struct container
{
    using null_t = std::nullptr_t;
    using bool_t = bool;
    using string_t = std::string;
    using int_t = int64_t;
    using double_t = double;
    using vector_t = std::vector<container>;
    using map_t = std::unordered_map<std::string, container>;
    using variant_t = std::variant<null_t, bool_t, string_t, int_t, double_t, vector_t, map_t>;

    template <typename T>
    bool is() const { return std::holds_alternative<T>(value); }

    template <typename T>
    T& as() { return std::get<T>(value); }

    container& operator[](const char* s)        { return as<map_t>()[s]; }
    container& operator[](const std::string& s) { return operator[](s.c_str()); }
    container& operator[](size_t i) { return as<vector_t>()[i]; }

    variant_t value;
};

container from_json(const std::string& s);
container to_json(const std::string& s);

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

template <typename T>
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
    }
    else
    {
        js.erase(k);
    }
}

template <typename T>
std::optional<T> create_object(web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));
    if (js.is_object() && js.has_field(k) && js.at(k).is_object())
    {
        return T { std::exchange(js.at(k), web::json::value { }) };
    }

    return std::nullopt;
}

template <typename T>
void object_to_json(web::json::value& js, std::string key, const std::optional<T>& obj)
{
    if (obj)
    {
        auto k = utility::conversions::to_string_t(std::move(key));
        js[k] = obj->to_json();
    }
}

template <typename T>
std::optional<std::vector<T>> create_object_list(web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));
    if (js.is_object() && js.has_field(k) && js.at(k).is_array())
    {
        std::optional<std::vector<T>> lst = std::vector<T> { };
        for (auto& i: js.at(k).as_array())
        {
            lst->emplace_back(std::exchange(i, web::json::value { }));
        }

        return lst;
    }

    return std::nullopt;
}

template <typename T>
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
