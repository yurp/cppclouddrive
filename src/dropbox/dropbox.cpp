
#include <ccd/dropbox/dropbox.h>

namespace ccd::dropbox
{
inline namespace v2
{

dropbox::dropbox(pplx::task<web::http::client::http_client_config> client_config)
    : m_client_config(std::move(client_config))
{

}

resource::files::files dropbox::files_resource()
{
    return resource::files::files{ m_client_config };
}

}
}
