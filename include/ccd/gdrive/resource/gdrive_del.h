
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

class del : public executor,
            public details::with_file_parameters<del>
{
    friend class files;

public:
    /// @brief The ID of the file
    del& set_file_id(std::string x);

    boost::future<void> exec();

private:
    del(boost::shared_future<std::string> token, std::string file_id);

    boost::future<executor::executor_ptr> build_request() override;

    boost::shared_future<std::string> m_token;
    std::string m_file_id;
};

}
}
}
