
#include <ccd/utils.h>

#include <cpprest/asyncrt_utils.h>
#include <cpprest/interopstream.h>
#include <cpprest/oauth2.h>

namespace ccd
{

using namespace web::http::oauth2::experimental;

namespace
{

void save_token(const std::string& oauth2_token_file, const oauth2_config& conf)
{
    web::json::value js = web::json::value::object();
    js["access_token"] = web::json::value{ conf.token().access_token() };
    js["refresh_token"] = web::json::value{ conf.token().refresh_token() };
    js["type"] = web::json::value{ conf.token().token_type() };

    if (auto f = utility::ofstream_t{ oauth2_token_file })
    {
        js.serialize(f);
    }
}

http_client_ptr create_client(const oauth2_config& conf)
{
    web::http::client::http_client_config http_conf;
    http_conf.set_oauth2(conf);
    return std::make_shared<web::http::client::http_client>("https://www.googleapis.com", http_conf);
}

}

pplx::task<http_client_ptr> create_gdrive_http_client(std::string oauth2_client_id,
                                                      std::string oauth2_client_secret,
                                                      std::string oauth2_token_file)
{
    auto conf = std::make_shared<oauth2_config>(std::move(oauth2_client_id),
                                                std::move(oauth2_client_secret),
                                                "https://accounts.google.com/o/oauth2/auth",
                                                "https://oauth2.googleapis.com/token",
                                                "urn:ietf:wg:oauth:2.0:oob",
                                                "https://www.googleapis.com/auth/drive");

    if (auto f = utility::ifstream_t { oauth2_token_file })
    {
        auto js = web::json::value::parse(f);
        oauth2_token token { js["access_token"].as_string() };
        token.set_refresh_token(js["refresh_token"].as_string());
        token.set_token_type(js["type"].as_string());

        conf->set_token(token);
        return conf->token_from_refresh().then([conf, oauth2_token_file = std::move(oauth2_token_file)]
        {
            if (!conf->token().refresh_token().empty())
            {
                save_token(oauth2_token_file, *conf);
            }
            return create_client(*conf);
        });
    }
    else
    {
        std::cout << "go to:" << conf->build_authorization_uri(true) << "\nthen paste input code here: ";

        static Concurrency::streams::stdio_istream<char> acin { std::cin };
        auto secret_code_buf = std::make_shared<Concurrency::streams::stringstreambuf>();
        return acin.read_line(*secret_code_buf).then([secret_code_buf, conf](size_t)
        {
            return conf->token_from_code(secret_code_buf->collection());
        })
        .then([conf, oauth2_token_file = std::move(oauth2_token_file)]
        {
            save_token(oauth2_token_file, *conf);
            return create_client(*conf);
        });
    }
}

namespace details
{

std::optional<string_list_t> create_string_list(web::json::value& js, std::string key)
{
    auto k = utility::conversions::to_string_t(std::move(key));
    if (js.is_object() && js.has_field(k) && js.at(k).is_array())
    {
        std::optional<string_list_t> lst = string_list_t{ };
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
        std::optional<key_value_list_t> lst = key_value_list_t{ };
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
    } else
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
    } else
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
