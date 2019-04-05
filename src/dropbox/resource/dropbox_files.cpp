
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

copy files::copy_request(std::string from_path, std::string to_path)
{
    return copy { m_http_transport_factory(), from_path, to_path };
}

create_folder files::create_folder_request(std::string path)
{
    return create_folder { m_http_transport_factory(), path };
}

download files::download_request(std::string path)
{
    return download { m_http_transport_factory(), std::move(path) };
}

get_metadata files::get_metadata_request(std::string path)
{
    return get_metadata { m_http_transport_factory(), std::move(path) };
}

del files::del_request(std::string path)
{
    return del { m_http_transport_factory(), std::move(path) };
}

list_folder files::list_folder_request(std::string path)
{
    return list_folder { m_http_transport_factory(), std::move(path) };
}

list_folder_continue files::list_folder_continue_request(std::string cursor)
{
    return list_folder_continue { m_http_transport_factory(), std::move(cursor) };
}

move files::move_request(std::string from_path, std::string to_path)
{
    return move { m_http_transport_factory(), std::move(from_path), std::move(to_path) };
}

upload files::upload_request(std::string path, std::string content)
{
    return upload { m_http_transport_factory(), std::move(path), std::move(content) };
}

}
}
}
