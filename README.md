# C++ Cloud Drive (CCD)

Async C++17 API for cloud drives. Supports Google Drive

## Google Drive

Implements Google Drive v3 API

The API structure is similar to official Java Google Drive API ([see here](https://developers.google.com/resources/api-libraries/documentation/drive/v3/java/latest/))

More info about filters, fields etc can be found in [official documentation](https://developers.google.com/drive/api/v3/reference/)

Usage example (list Google Drive root directory):
```c++
    ccd::auth::oauth2::automatic(GDRIVE_APP_ID, GDRIVE_SECRET_KEY, "http://localhost:25000/")
        .then([](boost::future<ccd::auth::oauth2::token> t)
        {
           ccd::http::authorized_oauth2_transport_factory f { t.get().access, ccd::http::cpprest_transport_factory{} };
           return ccd::gdrive::gdrive { std::move(f) };
        })
        .then([](boost::future<ccd::gdrive::gdrive> g))
        {
            auto files_rsc = g.get().files_resource();
            return files_rsc.list_request().set_page_size(999)
                                           .set_fields("files(id,name,mimeType,size,createdTime)")
                                           .set_q("'root' in parents")
                                           .exec();
        })
        .unwrap().then([](boost::future<ccd::gdrive::model::file_list> lst)
        {
            if (auto files = lst.get().get_files())
            {
                std::cout << "gdrive root directory:\n";
                for (const auto& file: *files)
                {
                    std::cout << "id = " << file.get_id().value_or("?")
                              << ", name = " << file.get_name().value_or("?")
                              << ", mime type = " << file.get_mime_type().value_or("?")
                              << ", size = " << file.get_size().value_or(-1)
                              << ", created = " << file.get_created_time().value_or("?") << "\n";
                }
            }
        })
        .get();
```
