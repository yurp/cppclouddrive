
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

struct var
{
    using null_t = std::nullptr_t;
    using bool_t = bool;
    using string_t = std::string;
    using int_t = int64_t;
    using double_t = double;
    using vector_t = std::vector<var>;
    using map_t = std::unordered_map<std::string, var>;
    using variant_t = std::variant<null_t, bool_t, string_t, int_t, double_t, vector_t, map_t>;

    var() : x(nullptr) {}
    var(const var& v) : x(v.x) {}
    var(var&& v) : x(std::move(v.x)) {}
    var(null_t ) : x(nullptr) {}
    var(bool_t x) : x(x) {}
    var(string_t x) : x(std::move(x)) {}
    var(const char* x) : x(string_t(x)) {}
    var(int_t x) : x(x) {}
    var(int x) : x(int64_t(x)) {}
    var(unsigned x) : x(int64_t(x)) {}
    var(uint64_t x) : x(int64_t(x)) {}
    var(double_t x) : x(x) {}
    var(vector_t x) : x(std::move(x)) {}
    var(map_t x) : x(std::move(x)) {}
    var(variant_t x) : x(std::move(x)) {}

    var& operator=(const var& y) { x = y.x; return *this; }
    var& operator=(var&& y) { x = std::move(y.x); return *this; }
    var& operator=(const null_t& ) { x = nullptr; return *this; }
    var& operator=(bool_t y) { x = y; return *this; }
    var& operator=(string_t y) { x = (std::move(y)) ; return *this; }
    var& operator=(const char* y) { x = string_t(y); return *this; }
    var& operator=(int_t y) { x = y; return *this; }
    var& operator=(int y) { x = int64_t(y); return *this; }
    var& operator=(unsigned y) { x = int64_t(y); return *this; }
    var& operator=(uint64_t y) { x = int64_t(y); return *this; }
    var& operator=(double_t y) { x = y; return *this; }
    var& operator=(vector_t y) { x = std::move(y); return *this; }
    var& operator=(map_t y) { x = std::move(y) ; return *this; }

    template <typename T>
    bool is() const { return std::holds_alternative<T>(x); }

    template <typename T>
    T& as() { return std::get<T>(x); }

    template <typename T>
    const T& as() const { return std::get<T>(x); }

    bool has(const std::string& s) const { return as<map_t>().count(s) > 0; }

    var& operator[](const char* s)        { return as<map_t>()[s]; }
    var& operator[](const std::string& s) { return operator[](s.c_str()); }
    var& operator[](size_t i) { return as<vector_t>()[i]; }
    var& operator[](int i) { return as<vector_t>()[i]; }

    variant_t x;
};

var from_json(const std::string& s);
std::string to_json(const var& v);

namespace details
{

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

template <typename T>
void object_to_json(ccd::var& js, const std::string& key, const std::optional<T>& obj)
{
    if (obj)
    {
        js[key] = obj->to_json();
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

template <typename T>
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
}

}
}
