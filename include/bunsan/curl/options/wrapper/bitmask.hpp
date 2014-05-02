#pragma once

#include <bunsan/curl/options/traits.hpp>

#include <curl/curl.h>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <typename T, T Default>
    class bitmask
    {
    public:
        typedef copy_policy::by_curl copy_policy;

    public:
        bitmask()=default;

        template <typename ... Args>
        bitmask(const T arg, Args &&...args):
            bitmask(static_cast<long>(arg), std::forward<Args>(args)...) {}

        inline long data() const
        {
            return m_data;
        }

    private:
        bitmask(const long data): m_data(data) {}

        template <typename ... Args>
        bitmask(const long arg1, const T arg2, Args &&...args):
            bitmask(
                arg1 | static_cast<long>(arg2),
                std::forward<Args>(args)...
            ) {}

    private:
        long m_data = static_cast<long>(Default);
    };
}}}}
