
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/gdrive/model/gdrive_file.h>
#include <ccd/gdrive/resource/gdrive_copy.h>
#include <ccd/gdrive/resource/gdrive_create.h>
#include <ccd/gdrive/resource/gdrive_del.h>
#include <ccd/gdrive/resource/gdrive_get.h>
#include <ccd/gdrive/resource/gdrive_list.h>
#include <ccd/gdrive/resource/gdrive_update.h>

namespace ccd::gdrive
{
inline namespace v3
{

class gdrive;

namespace resource::files
{

class files
{
    friend class ccd::gdrive::v3::gdrive;

public:
    list list_request();

    get get_request(std::string file_id);

    create create_request(model::file metadata);

    create create_request(model::file metadata, std::string media_content);

    copy copy_request(std::string file_id, model::file metadata_patch);

    del del_request(std::string file_id);

    update update_request(std::string file_id, model::file metadata_patch, std::string media_content);

    update update_request(std::string file_id, model::file metadata_patch);

    update update_request(std::string file_id, std::string media_content);

    update update_request(std::string file_id);

private:
    explicit files(pplx::task<http_client_ptr> client);

    pplx::task<http_client_ptr> m_client;
};

}
}
}
