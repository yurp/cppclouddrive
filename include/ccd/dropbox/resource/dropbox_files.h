
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/dropbox/resource/dropbox_copy.h>
#include <ccd/dropbox/resource/dropbox_create_folder.h>
#include <ccd/dropbox/resource/dropbox_delete.h>
#include <ccd/dropbox/resource/dropbox_download.h>
#include <ccd/dropbox/resource/dropbox_get_metadata.h>
#include <ccd/dropbox/resource/dropbox_list_folder.h>
#include <ccd/dropbox/resource/dropbox_move.h>

namespace ccd::dropbox
{
inline namespace v2
{

class dropbox;

namespace resource::files
{

class files
{
    friend class ccd::dropbox::v2::dropbox;

public:
    copy copy_request(std::string from_path, std::string to_path);
    create_folder create_folder_request(std::string path);
    del del_request(std::string path);
    download download_request(std::string path);
    get_metadata get_metadata_request(std::string path);
    list_folder list_folder_request(std::string path = "");
    list_folder_continue list_folder_continue_request(std::string cursor);
    move move_request(std::string from_path, std::string to_path);

private:
    files(ccd::http::transport_factory factory);

    ccd::http::transport_factory m_http_transport_factory;
};

}
}
}
