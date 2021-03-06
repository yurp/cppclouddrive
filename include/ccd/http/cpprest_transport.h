
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/http/http_transport.h>

namespace ccd::http
{

class cpprest_transport_factory
{
public:
    transport_func operator()();
};

}
