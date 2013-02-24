#include "bunsan/curl/easy.hpp"
#include "bunsan/curl/error.hpp"

#include <boost/assert.hpp>

namespace bunsan{namespace curl
{
    easy::easy(CURL *const curl) noexcept: m_curl(curl)
    {
        BOOST_ASSERT(curl);
    }

    easy::easy(): m_curl(::curl_easy_init())
    {
        if (!m_curl)
            BOOST_THROW_EXCEPTION(easy_error() <<
                                  easy_error::what_message("curl_easy_init"));
    }

    easy::~easy()
    {
        ::curl_easy_cleanup(m_curl);
    }
}}
