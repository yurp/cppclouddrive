
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/details/http_executor.h>
#include <ccd/dropbox/model/dropbox_metadata.h>

namespace ccd::dropbox
{
inline namespace v2
{
namespace resource::files
{

class files;

class create_folder : public ccd::details::http_executor
{
    friend class files;

public:
    /// @brief If there's a conflict, have the Dropbox server try to autorename the file to avoid the conflict.
    ///        The default for this field is False.
    create_folder& set_autorename(bool x);

    boost::future<model::metadata> exec();

private:
    /// @param[in] path (pattern="(/(.|[\r\n])*)|(ns:[0-9]+(/.*)?)") Path in the user's Dropbox to create.
    create_folder(ccd::http::transport_func http_transport, std::string path);

    ccd::http::request build_request() override;

    var m_json;
};

}
}
}
