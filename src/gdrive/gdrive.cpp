
#include <ccd/gdrive/gdrive.h>

namespace ccd::gdrive
{
inline namespace v3
{

gdrive::gdrive(ccd::http::authorized_oauth2_transport_factory http_transport_factory)
    : m_http_transport_factory(std::move(http_transport_factory))
{

}

resource::files::files gdrive::files_resource()
{
    return resource::files::files{ m_http_transport_factory };
}

}
}
