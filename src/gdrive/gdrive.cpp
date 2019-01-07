
#include <ccd/gdrive/gdrive.h>

namespace ccd::gdrive
{
inline namespace v3
{

gdrive::gdrive(pplx::task <http_client_ptr> client)
    : m_client(std::move(client))
{

}

resource::files::files gdrive::files_resource()
{
    return resource::files::files{ m_client };
}

}
}
