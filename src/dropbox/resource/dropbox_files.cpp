
#include <ccd/dropbox/resource/dropbox_files.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

namespace
{

pplx::task<http_client_ptr> build_client(std::string uri, pplx::task<web::http::client::http_client_config> config)
{
    return config.then([uri = std::move(uri)](web::http::client::http_client_config c) mutable
    {
        return std::make_shared<web::http::client::http_client>(std::move(uri), c);
    });
}

}

files::files(pplx::task<web::http::client::http_client_config> client_config)
    : m_client_config(std::move(client_config))
{
}

list_folder files::list_folder_request(std::string path)
{
    return list_folder { build_client("https://api.dropboxapi.com", m_client_config), std::move(path) };
};

list_folder_continue files::list_folder_continue_request(std::string cursor)
{
    return list_folder_continue { build_client("https://api.dropboxapi.com", m_client_config), std::move(cursor) };
}

}
}
}
