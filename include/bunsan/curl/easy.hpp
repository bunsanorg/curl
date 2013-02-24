#pragma once

#include <curl/curl.h>

#include <boost/noncopyable.hpp>

namespace bunsan{namespace curl
{
    class easy: private boost::noncopyable // TODO really?
    {
    public:
        /// \note Takes ownership.
        explicit easy(CURL *const curl) noexcept;

        easy();
        ~easy();

    private:
        CURL *const m_curl;
    };
}}
