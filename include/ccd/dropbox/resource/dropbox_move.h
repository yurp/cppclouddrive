
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

class move : public ccd::details::http_executor
{
    friend class files;

public:
    /// @brief If true, copy will copy contents in shared folder, otherwise RelocationError.cant_copy_shared_folder
    ///        will be returned if from_path contains shared folder. This field is always true for move. The default
    ///        for this field is False.
    move& set_allow_shared_folder(bool x);

    /// @brief  If there's a conflict, have the Dropbox server try to autorename the file to avoid the conflict.
    ///         The default for this field is False.
    move& set_autorename(bool x);

    /// @brief Allow moves by owner even if it would result in an ownership transfer for the content being moved.
    ///        This does not apply to copies. The default for this field is False.
    move& set_allow_ownership_transfer(bool x);

    boost::future<model::metadata> exec();

private:
    /// @param[in] from_path (pattern="(/(.|[\r\n])*)|(ns:[0-9]+(/.*)?)|(id:.*)") Path in the user's Dropbox to be
    ///                      copied or moved.
    /// @param[in] to_path   (pattern="(/(.|[\r\n])*)|(ns:[0-9]+(/.*)?)|(id:.*)") Path in the user's Dropbox that is
    ///                      the destination.
    move(ccd::http::transport_func http_transport, std::string from_path, std::string to_path);

    ccd::http::request build_request() override;

    var m_json;
};

}
}
}
