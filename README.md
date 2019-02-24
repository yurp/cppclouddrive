# C++ Cloud Drive (CCD)

Async C++17 API for cloud drives. Supports Google Drive. Implemented using [cpprestsdk](https://github.com/Microsoft/cpprestsdk)

## Google Drive

Implements Google Drive v3 API

The API structure is similar to official Java Google Drive API ([see here](https://developers.google.com/resources/api-libraries/documentation/drive/v3/java/latest/))

More info about filters, fields etc can be found in [official documentation](https://developers.google.com/drive/api/v3/reference/)

Usage example (list Google Drive root directory):
```c++
    auto http_client = gdrive::auth_auto(GDRIVE_APP_ID, GDRIVE_SECRET_KEY, "http://localhost:25000/")
        .then([token_file](http_client_config c)
        {
            return std::make_shared<web::http::client::http_client>("https://www.googleapis.com", c);
        }
    auto http_client = ccd::create_gdrive_http_client(GDRIVE_APP_ID, GDRIVE_SECRET_KEY, token_file);
    ccd::gdrive::gdrive g { http_client };
    auto files_rsc = g.files_resource();
    
    auto file_list = files_rsc.list_request().set_page_size(999)
                                             .set_fields("files(id,name,mimeType,size,createdTime)")
                                             .set_q("'root' in parents")
                                             .exec().get();

    if (auto files = file_list.get_files())
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
```
