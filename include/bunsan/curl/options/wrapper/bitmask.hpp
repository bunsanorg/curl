#pragma once

#include <bunsan/curl/options/traits.hpp>

#include <curl/curl.h>

#include <boost/cast.hpp>

#include <type_traits>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    namespace detail
    {
        template <typename T>
        typename std::enable_if<!std::is_enum<T>::value, long>::
        type bitmask_cast(const T data)
        {
            return boost::numeric_cast<long>(data);
        }

        template <typename T>
        typename std::enable_if<std::is_enum<T>::value, long>::
        type bitmask_cast(const T data)
        {
            return bitmask_cast(
                static_cast<
                    typename std::underlying_type<T>::type
                >(data)
            );
        }
    }

    template <typename T, T Default>
    class bitmask
    {
    public:
        typedef copy_policy::by_curl copy_policy;

    public:
        bitmask()=default;

        template <typename ... Args>
        bitmask(const T arg, Args &&...args):
            bitmask(
                detail::bitmask_cast(arg),
                std::forward<Args>(args)...
            ) {}

        inline long data() const
        {
            return m_data;
        }

    private:
        bitmask(const long data): m_data(data) {}

        template <typename ... Args>
        bitmask(const long arg1, const T arg2, Args &&...args):
            bitmask(
                arg1 | detail::bitmask_cast(arg2),
                std::forward<Args>(args)...
            ) {}

    private:
        long m_data = detail::bitmask_cast(Default);
    };
}}}}
