#pragma once

#include <bunsan/curl/options/traits.hpp>

#include <curl/curl.h>

#include <boost/cast.hpp>

#include <type_traits>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    class long_
    {
    public:
        typedef copy_policy::by_curl copy_policy;

        explicit long_(const long data): m_data(data) {}

        inline long data() const
        {
            return m_data;
        }

    private:
        long m_data;
    };
}}}}
