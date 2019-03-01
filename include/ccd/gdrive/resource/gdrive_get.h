
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

    boost::future<std::string> exec_media();

    boost::future<std::string> exec_media(int64_t offset, int64_t sz);

    boost::future<model::file> exec();

private:
    get(boost::shared_future<std::string> token, std::string file_id);

    boost::future<executor::executor_ptr> build_request() override;

    boost::shared_future<std::string> m_token;
    std::string m_file_id;
    std::optional<bool> m_acknowledge_abuse;
    std::optional<std::pair<int64_t, int64_t>> m_range;
};

}
}
}
