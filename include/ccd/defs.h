
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <cpprest/http_client.h>

#include <memory>
#include <string>
#include <vector>

namespace ccd
{

using key_value_list_t = std::vector<std::pair<std::string, std::string>>;
using string_list_t = std::vector<std::string>;
using timestamp_t = std::string;

}
