
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/executor.h>
#include <ccd/dropbox/model/dropbox_metadata.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

class files;

class download : public executor
{
    friend class files;

public:
    download& set_range(int64_t offset, int64_t sz);
    download& reset_range();

    pplx::task<std::string> exec();

private:
    /// @param[in] (pattern="(/(.|[\r\n])*|id:.*)|(rev:[0-9a-f]{9,})|(ns:[0-9]+(/.*)?)") The path of the file to
    ///            download.
    download(pplx::task<http_client_ptr> client, std::string path);

    web::http::http_request build_request() override;

    web::json::value m_json;
    std::optional<std::pair<int64_t, int64_t>> m_range;
};

}
}
}
