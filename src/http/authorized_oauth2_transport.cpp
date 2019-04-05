
#include <ccd/http/authorized_oauth2_transport.h>

namespace ccd::http
{

namespace
{

class authorized_oauth2_transport
{
public:
    authorized_oauth2_transport(std::string access_token, transport_func transport)
        : m_access_token(std::move(access_token))
        , m_transport(std::move(transport))
    {

    }

    response_future operator()(request r)
    {
        r.headers.emplace_back("Authorization", "Bearer " + m_access_token);
        return m_transport(std::move(r));
    }

private:
    std::string m_access_token;
    transport_func m_transport;
};

}

authorized_oauth2_transport_factory::authorized_oauth2_transport_factory(std::string access_token,
                                                                         transport_factory factory)
    : m_access_token(std::move(access_token))
    , m_factory(std::move(factory))
{

}

transport_func authorized_oauth2_transport_factory::operator()()
{
    return authorized_oauth2_transport { m_access_token, m_factory() };
}

}
