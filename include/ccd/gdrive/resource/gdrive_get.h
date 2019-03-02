
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/details/http_executor.h>
#include <ccd/gdrive/details/gdrive_parameters.h>
#include <ccd/gdrive/model/gdrive_file.h>

namespace ccd::gdrive
{
inline namespace v3
{
namespace resource::files
{

class files;

class get : public ccd::details::http_executor,
            public details::with_file_parameters<get>
{
    friend class files;

public:
    /// @brief The ID of the file
    get& set_file_id(std::string file_id);

    /// @brief Whether the user is acknowledging the risk of downloading known malware or other abusive files
    get& set_acknowledge_abuse(std::optional<bool> x);

    boost::future<std::string> exec_media();

    boost::future<std::string> exec_media(int64_t offset, int64_t sz);

    boost::future<model::file> exec();

private:
    get(ccd::http::transport_func hexec, std::string file_id);

    ccd::http::request build_request() override;

    std::string m_file_id;
    std::optional<bool> m_acknowledge_abuse;
    std::optional<std::pair<int64_t, int64_t>> m_range;
};

}
}
}
