
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

class del : public ccd::details::http_executor,
            public details::with_file_parameters<del>
{
    friend class files;

public:
    /// @brief The ID of the file
    del& set_file_id(std::string x);

    boost::future<void> exec();

private:
    del(ccd::http::transport_func http_transport, std::string file_id);

    ccd::http::request build_request() override;

    std::string m_file_id;
};

}
}
}
