#pragma once

#include <bunsan/curl/options/traits.hpp>

#include <curl/curl.h>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <long False, long True>
    class bool_
    {
    public:
        typedef copy_policy::by_curl copy_policy;

    public:
        bool_()=default;

        explicit bool_(const bool data): m_data(data) {}

        inline long data() const
        {
            return m_data ? True : False;
        }

    private:
        bool m_data = False;
    };

    typedef bool_<0, 1> bool_01;
    typedef bool_<0, 2> bool_02;
}}}}
