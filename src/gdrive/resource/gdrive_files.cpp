
#include <ccd/gdrive/resource/gdrive_files.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

files::files(ccd::http::transport_factory factory)
    : m_http_transport_factory(std::move(factory))
{

}

list files::list_request()
{
    return list{ m_http_transport_factory() };
}

get files::get_request(std::string file_id)
{
    return get{ m_http_transport_factory(), std::move(file_id) };
}

create files::create_request(model::file metadata)
{
    return create{ m_http_transport_factory(), std::move(metadata), std::nullopt };
}

create files::create_request(model::file metadata, std::string media_content)
{
    return create{ m_http_transport_factory(), std::move(metadata), std::move(media_content) };
}

copy files::copy_request(std::string file_id, model::file metadata_patch)
{
    return copy{ m_http_transport_factory(), std::move(file_id), std::move(metadata_patch) };
}

del files::del_request(std::string file_id)
{
    return del{ m_http_transport_factory(), std::move(file_id) };
}

update files::update_request(std::string file_id, model::file metadata_patch, std::string media_content)
{
    return update{ m_http_transport_factory(), 
                   std::move(file_id), 
                   std::move(metadata_patch), 
                   std::move(media_content) };
}

update files::update_request(std::string file_id, model::file metadata_patch)
{
    return update{ m_http_transport_factory(), std::move(file_id), std::move(metadata_patch), std::nullopt };
}

update files::update_request(std::string file_id, std::string media_content)
{
    return update{ m_http_transport_factory(), std::move(file_id), std::nullopt, std::move(media_content) };
}

update files::update_request(std::string file_id)
{
    return update{ m_http_transport_factory(), std::move(file_id), std::nullopt, std::nullopt };
}

}
}
}
