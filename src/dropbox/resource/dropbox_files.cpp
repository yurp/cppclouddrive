
#include <ccd/dropbox/resource/dropbox_files.h>

#include <ccd/utils.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

files::files(ccd::http::transport_factory factory)
    : m_http_transport_factory(std::move(factory))
{
}

download files::download_request(std::string path)
{
    return download { m_http_transport_factory(), std::move(path) };
};

list_folder files::list_folder_request(std::string path)
{
    return list_folder { m_http_transport_factory(), std::move(path) };
};

list_folder_continue files::list_folder_continue_request(std::string cursor)
{
    return list_folder_continue { m_http_transport_factory(), std::move(cursor) };
}

}
}
}
