
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#define BOOST_THREAD_PROVIDES_FUTURE_UNWRAP
#include <boost/thread.hpp>

#include <memory>
#include <string>
#include <vector>

namespace ccd
{

using key_value_list_t = std::vector<std::pair<std::string, std::string>>;
using string_list_t = std::vector<std::string>;
using timestamp_t = std::string;

}
