
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

    pplx::task<void> exec();

private:
    del(pplx::task<http_client_ptr> client, std::string file_id);

    web::http::http_request build_request() override;

    std::string m_file_id;
};

}
}
}
