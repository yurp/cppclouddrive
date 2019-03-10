
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

class del : public ccd::details::http_executor
{
    friend class files;

public:
    /// @brief  (min_length=9, pattern="[0-9a-f]+")? Perform delete if given "rev" matches the existing file's latest
    ///         "rev". This field does not support deleting a folder. This field is optional.
    del& set_parent_rev(std::string x);

    boost::future<model::metadata> exec();

private:
    /// @param[in] path (pattern="(/(.|[\r\n])*)|(ns:[0-9]+(/.*)?)") Path in the user's Dropbox to create.
    del(ccd::http::transport_func http_transport, std::string path);

    ccd::http::request build_request() override;

    var m_json;
};

}
}
}
