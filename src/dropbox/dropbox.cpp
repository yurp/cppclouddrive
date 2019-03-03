
#include <ccd/dropbox/dropbox.h>

namespace ccd::dropbox
{
inline namespace v2
{

dropbox::dropbox(ccd::http::authorized_oauth2_transport_factory http_transport_factory)
    : m_http_transport_factory(std::move(http_transport_factory))
{

}

resource::files::files dropbox::files_resource()
{
    return resource::files::files{ m_http_transport_factory };
}

}
}
