
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

struct use_boost_future_ec_t
{
    std::function<boost::system::error_code(boost::system::error_code)> error_processor;
};

namespace details::asio_use_boost_future
{

template<typename> struct handler_selector;

template<> struct handler_selector<void(boost::system::error_code)>
{
    using future_t = boost::future<void>;
    using error_processor_t = std::function<boost::system::error_code(boost::system::error_code)>;

    boost::promise<void> m_promise;
    error_processor_t m_error_processor;

    explicit handler_selector(error_processor_t f = error_processor_t{}) : m_error_processor(std::move(f)) {}

    void operator()(boost::system::error_code ec)
    {
        if (m_error_processor)
            ec = m_error_processor(ec);

        ec ? m_promise.set_exception(boost::system::system_error{ ec })
           : m_promise.set_value();
    }
};

template<typename T> struct handler_selector<void(boost::system::error_code, T)>
{
    using future_t = boost::future<T>;
    using error_processor_t = std::function<boost::system::error_code(boost::system::error_code)>;

    boost::promise<T> m_promise;
    error_processor_t m_error_processor;

    explicit handler_selector(error_processor_t f = error_processor_t{}) : m_error_processor(std::move(f)) {}

    void operator()(boost::system::error_code ec, T t) // TODO: why can't it compile T&& ???
    {
        if (m_error_processor)
            ec = m_error_processor(ec);

        ec ? m_promise.set_exception(boost::system::system_error{ ec })
           : m_promise.set_value(std::forward<T>(t));
    }
};

template<typename... Args> struct handler_selector<void(boost::system::error_code, Args...)>
{
    using future_t = boost::future<std::tuple<Args...>>;
    using error_processor_t = std::function<boost::system::error_code(boost::system::error_code)>;

    boost::promise<std::tuple<Args...>> m_promise;
    error_processor_t m_error_processor;

    explicit handler_selector(error_processor_t f = error_processor_t{}) : m_error_processor(std::move(f)) {}

    void operator()(boost::system::error_code ec, Args&& ... args)
    {
        if (m_error_processor)
            ec = m_error_processor(ec);

        ec ? m_promise.set_exception(boost::system::system_error{ ec })
           : m_promise.set_value(std::forward_as_tuple(args...));
    }
};

template<typename Signature>
struct handler : handler_selector<Signature>
{
    handler(const use_boost_future_t&) {}
    handler(const use_boost_future_ec_t& t) : handler_selector<Signature>(t.error_processor) {}
};

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

template<typename Signature>
class async_result<ccd::use_boost_future_ec_t, Signature>
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
