
#pragma once

// (c) 2019 Iurii Pelykh
// This code is licensed under MIT license

#include <ccd/http/http_transport.h>

#include <boost/asio.hpp>

namespace ccd::http
{

transport_func beast_transport_factory();

struct async_beast_transport_factory
{
    explicit async_beast_transport_factory(boost::asio::io_context& ioc);
    transport_func operator()();

private:
    boost::asio::io_context& m_ioc;
};

}
