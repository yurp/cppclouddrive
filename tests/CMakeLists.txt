cmake_minimum_required(VERSION 3.12)

project(ccd_tests LANGUAGES CXX)

set(GOOGLETEST_SRC
        libs/googletest/googletest/src/gtest_main.cc
        libs/googletest/googletest/src/gtest-all.cc
        libs/googletest/googlemock/src/gmock-all.cc)

set(TESTS_SRC
        utils_test.cpp)

set(GDRIVE_TESTS_SRC
        gdrive/model/gdrive_user_test.cpp)

set(HTTP_TESTS_SRC
        http/beast_transport_test.cpp)

add_executable(ccd_tests ${TESTS_SRC} ${GDRIVE_TESTS_SRC} ${HTTP_TESTS_SRC} ${GOOGLETEST_SRC})
target_link_libraries(ccd_tests
        ccd)
target_include_directories(ccd_tests
        PRIVATE
        libs/googletest/googletest
        libs/googletest/googlemock
        libs/googletest/googletest/include
        libs/googletest/googlemock/include)
