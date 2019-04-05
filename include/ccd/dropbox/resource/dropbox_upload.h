
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

class upload : public ccd::details::http_executor
{
    friend class files;

public:
    /// @brief Do not overwrite an existing file if there is a conflict. The autorename strategy is to append a
    ///        number to the file name. For example, "document.txt" might become "document (2).txt".
    upload& set_add_mode();

    /// @brief Always overwrite the existing file. The autorename strategy is the same as it is for add.
    upload& set_overwrite_mode();

    /// @brief String(min_length=9, pattern="[0-9a-f]+") Overwrite if the given "rev" matches the existing file's "rev".
    ///        The autorename strategy is to append the string "conflicted copy" to the file name. For example,
    ///        "document.txt" might become "document (conflicted copy).txt" or "document (Panda's conflicted copy).txt".
    upload& set_update_mode(std::string x);

    /// @brief If there's a conflict, as determined by mode, have the Dropbox server try to autorename the file to avoid
    ///        conflict. The default for this field is False.
    upload& set_autorename(bool x);

    /// @brief (format="%Y-%m-%dT%H:%M:%SZ")? The value to store as the client_modified timestamp. Dropbox automatically
    ///        records the time at which the file was written to the Dropbox servers. It can also record an additional
    ///        timestamp, provided by Dropbox desktop clients, mobile clients, and API apps of when the file was
    ///        actually created or modified. This field is optional.
    upload& set_client_modified(std::string x);

    /// @brief  Normally, users are made aware of any file modifications in their Dropbox account via notifications in
    ///         the client software. If true, this tells the clients that this modification shouldn't result in a user
    ///         notification. The default for this field is False.
    upload& set_mute(bool x);

    /// @brief Be more strict about how each WriteMode detects conflict. For example, always return a conflict error
    ///        when mode = WriteMode.update and the given "rev" doesn't match the existing file's "rev", even if the
    ///        existing file has been deleted. The default for this field is False.
    upload& set_strict_conflict(bool x);

    // TODO: add property_groups

    boost::future<model::metadata> exec();

private:
    /// @param[in] (pattern="(/(.|[\r\n])*)|(ns:[0-9]+(/.*)?)|(id:.*)") Path in the user's Dropbox to save the file.
    upload(ccd::http::transport_func http_transport, std::string path, std::string content);

    ccd::http::request build_request() override;

    var m_json;
    std::string m_content;
};

}
}
}
