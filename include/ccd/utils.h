
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/defs.h>
#include <ccd/var.h>

#include <optional>

namespace ccd
{

var from_json(const std::string& s);
std::string to_json(const var& v);

std::optional<string_list_t> create_string_list(ccd::var& js, const std::string& key);
void string_list_to_json(ccd::var& js, const std::string& key, const std::optional<string_list_t>& lst);
std::optional<key_value_list_t> create_key_value_list(ccd::var& js, const std::string& key);
void key_value_list_to_json(ccd::var& js, const std::string& key, const std::optional<key_value_list_t>& lst);

template <typename T>
std::optional<T> get_value(const ccd::var& js, const std::string& key)
{
    if (js.is<ccd::var::map_t>() && js.has(key) && js.as<ccd::var::map_t>().at(key).is<T>())
    {
        return js.as<ccd::var::map_t>().at(key).as<T>();
    }

    return std::nullopt;
}

template <typename T>
void set_value(ccd::var& js, const std::string& key, std::optional<T> value)
{
    if (js.is<ccd::var::map_t>())
    {
        if (!value)
        {
            js.as<ccd::var::map_t>().erase(key);
            return;
        }
        js[key] = *value;
    }
}

template <typename T>
std::optional<T> create_object(ccd::var& js, const std::string& key)
{
    if (js.is<ccd::var::map_t>() && js.has(key) && js[key].is<ccd::var::map_t>())
    {
        return T { std::exchange(js[key], ccd::var { }) };
    }

    return std::nullopt;
}

template<typename T>
std::optional<T> create_object_copy(const ccd::var& js, const std::string& key)
{
    if (js.is<var::map_t>() && js.has(key) && js.as<var::map_t>().at(key).is<var::map_t>())
    {
        return T{ js.as<var::map_t>().at(key) };
    }

    return std::nullopt;
}

template<typename T>
void object_to_json(ccd::var& js, const std::string& key, const std::optional<T>& obj)
{
    if (obj)
    {
        js[key] = obj->to_json();
    }
    else if (js.has(key))
    {
        js.as<ccd::var::map_t>().erase(key);
    }
}

template <typename T>
std::optional<std::vector<T>> create_object_list(ccd::var& js, const std::string& key)
{
    if (js.is<ccd::var::map_t>() && js.has(key) && js[key].is<ccd::var::vector_t>())
    {
        std::optional<std::vector<T>> lst = std::vector<T> { };
        for (auto& i: js[key].as<ccd::var::vector_t>())
        {
            lst->emplace_back(std::exchange(i, ccd::var{}));
        }

        return lst;
    }

    return std::nullopt;
}

template<typename T>
std::optional<std::vector<T>> create_object_list_copy(const ccd::var& js, const std::string& key)
{
    if (js.is<var::map_t>() && js.has(key) && js.as<var::map_t>().at(key).is<var::map_t>())
    {
        std::optional<std::vector<T>> lst = std::vector<T>{ };
        for (auto& i: js.as<var::map_t>().at(key).as<ccd::var::vector_t>())
        {
            lst->emplace_back(i);
        }

        return lst;
    }

    return std::nullopt;
}

template<typename T>
void object_list_to_json(ccd::var& js, const std::string& key, const std::optional<std::vector<T>>& lst)
{
    if (lst)
    {
        js[key] = ccd::var::vector_t{};
        auto& js_arr = js[key].as<ccd::var::vector_t>();
        for (const auto& i: *lst)
        {
            js_arr.emplace_back(i.to_json());
        }
    }
    else if (js.has(key))
    {
        js.as<var::map_t>().erase(key);
    }
}

}
