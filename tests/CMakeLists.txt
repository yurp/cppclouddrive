cmake_minimum_required(VERSION 3.12)

project(ccd_tests LANGUAGES CXX)

set(GOOGLETEST_SRC
        libs/googletest/googletest/src/gtest-all.cc
        libs/googletest/googletest/src/gtest_main.cc)

set(GDRIVE_TESTS_SRC
        gdrive/model/gdrive_user_test.cpp)

add_executable(gdrive_tests ${GDRIVE_TESTS_SRC} ${GOOGLETEST_SRC})
target_link_libraries(gdrive_tests
        ccd)
target_include_directories(gdrive_tests
        PRIVATE
        libs/googletest/googletest
        libs/googletest/googletest/include)
