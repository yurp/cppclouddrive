
#include <ccd/gdrive/gdrive.h>

namespace ccd::gdrive
{
inline namespace v3
{

gdrive::gdrive(boost::shared_future<std::string> token)
    : m_token(std::move(token))
{

}

resource::files::files gdrive::files_resource()
{
    return resource::files::files{ m_token };
}

}
}
