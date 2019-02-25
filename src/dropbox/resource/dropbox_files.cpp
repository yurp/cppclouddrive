
#include <ccd/dropbox/resource/dropbox_files.h>

#include <ccd/utils.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

files::files(pplx::task<web::http::client::http_client_config> client_config)
    : m_client_config(std::move(client_config))
{
}

download files::download_request(std::string path)
{
    return download { details::build_client("https://content.dropboxapi.com", m_client_config),
                      std::move(path) };
};

list_folder files::list_folder_request(std::string path)
{
    return list_folder { details::build_client("https://api.dropboxapi.com", m_client_config),
                         std::move(path) };
};

list_folder_continue files::list_folder_continue_request(std::string cursor)
{
    return list_folder_continue { details::build_client("https://api.dropboxapi.com", m_client_config),
                                  std::move(cursor) };
}

}
}
}
