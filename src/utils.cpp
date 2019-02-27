
#include <ccd/utils.h>

#include <rapidjson/reader.h>

#include <mutex>

namespace ccd
{

container from_json(const std::string& s)
{
    using namespace std;
    using namespace rapidjson;

    struct handler
    {
        bool Null()             { stack.back()->value = nullptr;                 finish(); return true; }
        bool Bool(bool b)       { stack.back()->value = b;                       finish(); return true; }
        bool Int(int i)         { stack.back()->value = static_cast<int64_t>(i); finish(); return true; }
        bool Uint(unsigned u)   { stack.back()->value = static_cast<int64_t>(u); finish(); return true; }
        bool Int64(int64_t i)   { stack.back()->value = static_cast<int64_t>(i); finish(); return true; }
        bool Uint64(uint64_t u) { stack.back()->value = static_cast<int64_t>(u); finish(); return true; }
        bool Double(double d)   { stack.back()->value =  d;                      finish(); return true; }
        bool RawNumber(const char* str, SizeType length, bool copy) { return true; }

        bool String(const char* str, SizeType length, bool copy)
        {
            stack.back()->value = std::string(str, str + length);
            finish();
            return true;
        }
        bool StartObject()
        {
            stack.back()->value = container::map_t{};
            return true;
        }
        bool Key(const char* str, SizeType length, bool copy)
        {
            stack.push_back(&std::get<container::map_t>(stack.back()->value)[std::string(str, str + length)]);
            return true;
        }
        bool EndObject(SizeType memberCount)
        {
            finish();
            return true;
        }
        bool StartArray()
        {
            stack.back()->value = container::vector_t{};
            std::get<container::vector_t>(stack.back()->value).emplace_back();
            stack.push_back(&std::get<container::vector_t>(stack.back()->value).back());
            return true;
        }
        bool EndArray(SizeType elementCount)
        {
            stack.pop_back();
            std::get<container::vector_t>(stack.back()->value).pop_back();
            finish();
            return true;
        }

        void finish()
        {
            stack.pop_back();
            if (!stack.empty() && std::holds_alternative<container::vector_t>(stack.back()->value))
            {
                std::get<container::vector_t>(stack.back()->value).emplace_back();
                stack.push_back(&std::get<container::vector_t>(stack.back()->value).back());
            }
        }

        container c { nullptr };
        std::vector<container*> stack { &c };
    } h;

    Reader reader;
    StringStream ss(s.c_str());
    reader.Parse(ss, h);

    return h.c;
}

namespace details
{

std::optional<string_list_t> create_string_list(web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));
    if (js.is_object() && js.has_field(k) && js.at(k).is_array())
    {
        std::optional<string_list_t> lst = string_list_t { };
        for (auto& i: js.at(k).as_array())
        {
            lst->emplace_back(utility::conversions::to_utf8string(i.as_string()));
        }

        return lst;
    }

    return std::nullopt;
}

void string_list_to_json(web::json::value& js, std::string key, const std::optional<string_list_t>& lst)
{
    if (lst)
    {
        auto k = utility::conversions::to_string_t(std::move(key));
        js[k] = web::json::value::array();
        auto& js_arr = js[k];
        int j = 0;
        for (const auto& i: *lst)
        {
            js_arr[j++] = web::json::value(utility::conversions::to_string_t(i));
        }
    }
}

std::optional<key_value_list_t> create_key_value_list(web::json::value& js, std::string key)
{
    using namespace utility::conversions;
    auto k = to_string_t(std::move(key));
    if (js.is_object() && js.has_field(k) && js.at(k).is_array())
    {
        std::optional<key_value_list_t> lst = key_value_list_t { };
        for (auto& kv: js.at(k).as_object())
        {
            lst->push_back({ to_utf8string(kv.first), to_utf8string(kv.second.as_string()) });
        }

        return lst;
    }

    return std::nullopt;
}

void key_value_list_to_json(web::json::value& js, std::string key, const std::optional<key_value_list_t>& lst)
{
    if (lst)
    {
        auto k = utility::conversions::to_string_t(std::move(key));
        js[k] = web::json::value::object();
        auto& js_obj = js[k];
        for (const auto& i: *lst)
        {
            js_obj[i.first] = web::json::value(utility::conversions::to_string_t(i.second));
        }
    }
}


std::optional<std::string> get_string(const web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));

    if (js.is_object() && js.has_field(k) && js.at(k).is_string())
    {
        return utility::conversions::to_utf8string(js.at(k).as_string());
    }

    return std::nullopt;
}

void set_string(web::json::value& js, std::string key, std::optional<std::string> value)
{
    auto k = utility::conversions::to_string_t(std::move(key));

    if (!js.is_object())
    {
        return;
    }

    if (value)
    {
        js[k] = web::json::value::string(utility::conversions::to_string_t(*value));
    }
    else
    {
        js.erase(k);
    }
}

std::optional<bool> get_bool(const web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));

    if (js.is_object() && js.has_field(k) && js.at(k).is_boolean())
    {
        return js.at(k).as_bool();
    }

    return std::nullopt;
}

void set_bool(web::json::value& js, std::string key, std::optional<bool> value)
{
    auto k = utility::conversions::to_string_t(std::move(key));

    if (!js.is_object())
    {
        return;
    }

    if (value)
    {
        js[k] = web::json::value::boolean(*value);
    }
    else
    {
        js.erase(k);
    }
}

std::optional<double> get_double(const web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));

    if (js.is_object() && js.has_field(k))
    {
        if (js.at(k).is_double())
        {
            return js.at(k).as_double();
        }
        else
        {
            auto s = utility::conversions::to_utf8string(js.at(k).as_string());
            return std::stod(s);
        }
    }

    return std::nullopt;
}

std::optional<int64_t> get_int64(const web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));

    if (js.is_object() && js.has_field(k))
    {
        if (js.at(k).is_number())
        {
            return js.at(k).as_number().to_int64();
        }
        else
        {
            auto s = utility::conversions::to_utf8string(js.at(k).as_string());
            return std::stoll(s);
        }
    }

    return std::nullopt;
}

std::optional<int32_t> get_int32(const web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));

    if (js.is_object() && js.has_field(k))
    {
        if (js.at(k).is_number())
        {
            return js.at(k).as_number().to_int32();
        }
        else
        {
            auto s = utility::conversions::to_utf8string(js.at(k).as_string());
            return std::stoi(s);
        }
    }

    return std::nullopt;
}

}
}
