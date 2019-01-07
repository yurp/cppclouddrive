
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/executor.h>
#include <ccd/gdrive/details/gdrive_parameters.h>
#include <ccd/gdrive/model/gdrive_file.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

class files;

class get : public executor,
            public details::with_file_parameters<get>
{
    friend class files;

public:
    /// @brief The ID of the file
    get& set_file_id(std::string file_id);

    /// @brief Whether the user is acknowledging the risk of downloading known malware or other abusive files
    get& set_acknowledge_abuse(std::optional<bool> x);

    pplx::task<std::string> exec_media();

    pplx::task<std::string> exec_media(int64_t offset, int64_t sz);

    pplx::task<model::file> exec();

private:
    get(pplx::task<http_client_ptr> client, std::string file_id);

    web::http::http_request build_request() override;

    std::string m_file_id;
    std::optional<bool> m_acknowledge_abuse;
    std::optional<std::pair<int64_t, int64_t>> m_range;
};

}
}
}
