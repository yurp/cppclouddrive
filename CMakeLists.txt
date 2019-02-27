cmake_minimum_required(VERSION 3.12)

project(ccd LANGUAGES CXX VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS thread)
find_package(cpprestsdk REQUIRED)
find_package(RapidJSON REQUIRED)

option(CCD_BUILD_EXAMPLES "build examples" OFF)
option(CCD_BUILD_TESTS "build tests" OFF)

add_compile_options(-Wall -Werror)
add_definitions(-D_TURN_OFF_PLATFORM_STRING) # disable cpprestsdk U() macro, which may conflict with other libs

set(CCD_HDR
        include/ccd/auth_utils.h
        include/ccd/defs.h
        include/ccd/executor.h
        include/ccd/utils.h

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
        src/auth_utils.cpp
        src/executor.cpp
        src/utils.cpp

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
        Boost::thread
        cpprestsdk::cpprest)
target_include_directories(ccd
        PUBLIC
        include)

if(CCD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(CCD_BUILD_TESTS)
    add_subdirectory(tests)
endif()
