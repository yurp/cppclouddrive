cmake_minimum_required(VERSION 3.12)

project(ccd LANGUAGES CXX VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS thread)
find_package(cpprestsdk REQUIRED)
find_package(CURL REQUIRED)
find_package(RapidJSON REQUIRED)

option(CCD_BUILD_EXAMPLES "build examples" OFF)
option(CCD_BUILD_TESTS "build tests" OFF)

add_compile_options(-Wall -Werror)
add_definitions(-D_TURN_OFF_PLATFORM_STRING) # disable cpprestsdk U() macro, which may conflict with other libs

set(CCD_HDR
        include/ccd/auth.h
        include/ccd/defs.h
        include/ccd/utils.h
        include/ccd/var.h

        include/ccd/details/http_executor.h

        include/ccd/http/authorized_oauth2_transport.h
        include/ccd/http/beast_transport.h
        include/ccd/http/curl_transport.h # should be optional!
        include/ccd/http/http_transport.h
        include/ccd/http/cpprest_transport.h # should be optional!

        include/ccd/dropbox/dropbox.h
        include/ccd/dropbox/model/dropbox_metadata.h
        include/ccd/dropbox/resource/dropbox_download.h
        include/ccd/dropbox/resource/dropbox_files.h
        include/ccd/dropbox/resource/dropbox_list_folder.h

        include/ccd/gdrive/gdrive.h
        include/ccd/gdrive/details/gdrive_parameters.h
        include/ccd/gdrive/model/gdrive_file.h
        include/ccd/gdrive/model/gdrive_permission.h
        include/ccd/gdrive/model/gdrive_user.h
        include/ccd/gdrive/resource/gdrive_copy.h
        include/ccd/gdrive/resource/gdrive_create.h
        include/ccd/gdrive/resource/gdrive_del.h
        include/ccd/gdrive/resource/gdrive_files.h
        include/ccd/gdrive/resource/gdrive_get.h
        include/ccd/gdrive/resource/gdrive_list.h
        include/ccd/gdrive/resource/gdrive_update.h)
set(CCD_SRC
        src/auth.cpp
        src/utils.cpp
        
        src/details/http_executor.cpp

        src/http/authorized_oauth2_transport.cpp
        src/http/beast_transport.cpp
        src/http/curl_transport.cpp # should be optional!
        src/http/http_transport.cpp
        src/http/cpprest_transport.cpp # should be optional!

        src/dropbox/dropbox.cpp
        src/dropbox/model/dropbox_metadata.cpp
        src/dropbox/resource/dropbox_download.cpp
        src/dropbox/resource/dropbox_files.cpp
        src/dropbox/resource/dropbox_list_folder.cpp

        src/gdrive/gdrive.cpp
        src/gdrive/model/gdrive_file.cpp
        src/gdrive/model/gdrive_permission.cpp
        src/gdrive/model/gdrive_user.cpp
        src/gdrive/resource/gdrive_copy.cpp
        src/gdrive/resource/gdrive_create.cpp
        src/gdrive/resource/gdrive_del.cpp
        src/gdrive/resource/gdrive_files.cpp
        src/gdrive/resource/gdrive_get.cpp
        src/gdrive/resource/gdrive_list.cpp
        src/gdrive/resource/gdrive_update.cpp)

add_library(ccd ${CCD_HDR} ${CCD_SRC})
target_link_libraries(ccd
        PUBLIC
        Boost::system
        Boost::thread
        cpprestsdk::cpprest
        curl)
target_include_directories(ccd
        PUBLIC
        include)

if(CCD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(CCD_BUILD_TESTS)
    add_subdirectory(tests)
endif()
