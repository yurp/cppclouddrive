
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

template<class ...Ts>
using future_tuple = boost::future<std::tuple<boost::future<Ts>...>>;

using key_value_list_t = std::vector<std::pair<std::string, std::string>>;
using string_list_t = std::vector<std::string>;
using timestamp_t = std::string;

}

// ---------------------------------------------------------------------------------------------------------------------

namespace ccd
{

struct use_boost_future_t { };
constexpr use_boost_future_t ftr;

namespace details::asio_use_boost_future
{

template<typename> struct handler_selector;

template<> struct handler_selector<void(boost::system::error_code)>
{
    using future_t = boost::future<void>;
    boost::promise<void> m_promise;
    void operator()(boost::system::error_code ec)
    {
        ec ? m_promise.set_exception(boost::system::system_error{ ec })
           : m_promise.set_value();
    }
};

template<typename T> struct handler_selector<void(boost::system::error_code, T)>
{
    using future_t = boost::future<T>;
    boost::promise<T> m_promise;
    void operator()(boost::system::error_code ec, T&& t)
    {
        ec ? m_promise.set_exception(boost::system::system_error{ ec })
           : m_promise.set_value(std::forward<T>(t));
    }
};

template<typename... Args> struct handler_selector<void(boost::system::error_code, Args...)>
{
    using future_t = boost::future<std::tuple<Args...>>;
    boost::promise<std::tuple<Args...>> m_promise;
    void operator()(boost::system::error_code ec, Args&& ... args)
    {
        ec ? m_promise.set_exception(boost::system::system_error{ ec })
           : m_promise.set_value(std::forward_as_tuple(args...));
    }
};

template<typename Signature>
struct handler : handler_selector<Signature> { handler(const use_boost_future_t&) {} };

}

}

#include <boost/asio/async_result.hpp>

namespace boost::asio
{

template<typename Signature>
class async_result<ccd::use_boost_future_t, Signature>
{
public:
    using completion_handler_type = ccd::details::asio_use_boost_future::handler<Signature>;
    using return_type = typename completion_handler_type::future_t;
    explicit async_result(completion_handler_type& h) : m_future(h.m_promise.get_future()) { }
    return_type get() { return std::move(m_future); }

private:
    return_type m_future;
};

}
