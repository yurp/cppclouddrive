
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <string>
#include <map>  // TODO: unordered map is better but it produce recursive variant which is not valid
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
    using map_t = std::map<std::string, var>;
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

    bool operator==(bool_t y) const { return is<bool_t>() && as<bool_t>() == y; }
    bool operator==(const string_t& y) const { return is<string_t>() && as<string_t>() == y; }
    bool operator==(const char* y) const { return is<string_t>() && as<string_t>() == y; }
    bool operator==(int_t y) const { return is<int_t>() && as<int_t>() == y; }
    bool operator==(double_t y) const { return is<double_t>() && as<double_t>() == y; }

    variant_t x;
};

inline bool operator==(var::bool_t y, const var& x) { return x == y; }
inline bool operator==(const var::string_t& y, const var& x) { return x == y; }
inline bool operator==(const char* y, const var& x) { return x == y; }
inline bool operator==(var::int_t y, const var& x) { return x == y; }
inline bool operator==(var::double_t y, const var& x) { return x == y; }

}
