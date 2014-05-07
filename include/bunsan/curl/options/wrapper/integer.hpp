#pragma once

#include <bunsan/curl/options/traits.hpp>

#include <curl/curl.h>

#include <type_traits>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <typename T>
    class integer
    {
    public:
        typedef copy_policy::by_curl copy_policy;

        explicit integer(const T data): m_data(data) {}

        inline T data() const
        {
            return m_data;
        }

    private:
        T m_data;
    };

    typedef integer<long> long_;
    typedef integer<::curl_off_t> curl_off_t_;
    typedef integer<time_t> time_t_;
}}}}
