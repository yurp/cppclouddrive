cmake_minimum_required(VERSION 3.12)

project(examples LANGUAGES CXX)

add_executable(gdrive_examples gdrive_examples.cpp)
target_link_libraries(gdrive_examples ccd)
