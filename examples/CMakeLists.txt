cmake_minimum_required(VERSION 3.12)

project(examples LANGUAGES CXX)

add_executable(gdrive_examples gdrive_examples.cpp)
target_link_libraries(gdrive_examples ccd)

add_executable(dropbox_examples dropbox_examples.cpp)
target_link_libraries(dropbox_examples ccd)

add_executable(http_transport_examples http_transport_examples.cpp)
target_link_libraries(http_transport_examples ccd)
