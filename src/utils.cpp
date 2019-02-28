
#include <ccd/utils.h>

#include <rapidjson/reader.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>

#include <mutex>

namespace ccd
{

var from_json(const std::string& s)
{
    using namespace rapidjson;

    struct handler
    {
        bool Null()             { *stack.back() = nullptr; finish(); return true; }
        bool Bool(bool b)       { *stack.back() = b;       finish(); return true; }
        bool Int(int i)         { *stack.back() = i;       finish(); return true; }
        bool Uint(unsigned u)   { *stack.back() = u;       finish(); return true; }
        bool Int64(int64_t i)   { *stack.back() = i;       finish(); return true; }
        bool Uint64(uint64_t u) { *stack.back() = u;       finish(); return true; }
        bool Double(double d)   { *stack.back() = d;       finish(); return true; }
        bool RawNumber(const char*, SizeType, bool)                { return true; }

        bool String(const char* s, SizeType len, bool copy)
        {
            *stack.back() = std::string(s, s + len);
            finish();
            return true;
        }
        bool StartObject()      { *stack.back() = var::map_t{};      return true; }
        bool Key(const char* s, SizeType len, bool copy)
        {
            stack.push_back(&(*stack.back())[std::string(s, s + len)]);
            return true;
        }
        bool EndObject(SizeType memberCount)    {          finish(); return true; }
        bool StartArray()
        {
            *stack.back() = var::vector_t{};
            stack.back()->as<var::vector_t>().emplace_back();
            stack.push_back(&stack.back()->as<var::vector_t>().back());
            return true;
        }
        bool EndArray(SizeType elementCount)
        {
            stack.pop_back();
            stack.back()->as<var::vector_t>().pop_back();
            finish();
            return true;
        }

        void finish()
        {
            stack.pop_back();
            if (!stack.empty() && stack.back()->is<var::vector_t>())
            {
                stack.back()->as<var::vector_t>().emplace_back();
                stack.push_back(&stack.back()->as<var::vector_t>().back());
            }
        }

        var c { nullptr };
        std::vector<var*> stack { &c };
    } h;

    Reader reader;
    StringStream ss(s.c_str());
    reader.Parse(ss, h);

    return h.c;
}

std::string to_json(const var& x)
{
    using namespace rapidjson;

    struct visitor
    {
        void operator()(const var::null_t& )
        {
            w.Null();
        }

        void operator()(const var::bool_t& b)
        {
            w.Bool(b);
        }

        void operator()(const var::string_t& s)
        {
            w.String(s.c_str(), s.length());
        }

        void operator()(const var::int_t& i)
        {
            w.Int64(i);
        }

        void operator()(const var::double_t& d)
        {
            w.Double(d);
        }

        void operator()(const var::vector_t& v)
        {
            w.StartArray();
            for (const auto& i: v)
            {
                std::visit(*this, i.x);
            }
            w.EndArray();
        }

        void operator()(const var::map_t& m)
        {
            w.StartObject();
            for (const auto& kv: m)
            {
                w.Key(kv.first.c_str(), kv.first.length());
                std::visit(*this, kv.second.x);
            }
            w.EndObject();
        }

        StringBuffer sb;
        PrettyWriter<StringBuffer> w { sb };
    } v;

    std::visit(v, x.x);

    return v.sb.GetString();
}

namespace details
{

std::optional<string_list_t> create_string_list(ccd::var& js, const std::string& key)
{
    if (js.is<ccd::var::map_t>() && js.has(key) && js[key].is<ccd::var::vector_t>())
    {
        std::optional<string_list_t> lst = string_list_t { };
        for (auto& i: js[key].as<ccd::var::vector_t>())
        {
            lst->emplace_back(i.as<ccd::var::string_t>());
        }

        return lst;
    }

    return std::nullopt;
}

void string_list_to_json(ccd::var& js, const std::string& key, const std::optional<string_list_t>& lst)
{
    if (lst)
    {
        js[key] = ccd::var::vector_t{};
        auto& js_arr = js[key].as<ccd::var::vector_t>();
        for (const auto& i: *lst)
        {
            js_arr.emplace_back(i);
        }
    }
}

std::optional<key_value_list_t> create_key_value_list(ccd::var& js, const std::string& key)
{
    // TODO: implement properly
#if 0
    if (js.is<ccd::var::map_t>() && js.has(key) && js[key].is<ccd::var::vector_t>())
    {
        std::optional<key_value_list_t> lst = key_value_list_t { };
        for (auto& kv: js.at(k).as_object())
        {
            lst->push_back({ to_utf8string(kv.first), to_utf8string(kv.second.as_string()) });
        }

        return lst;
    }
#endif
    return std::nullopt;
}

void key_value_list_to_json(ccd::var& js, const std::string& key, const std::optional<key_value_list_t>& lst)
{
// TODO: implement properly
#if 0
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
#endif
}

}
}
