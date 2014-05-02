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
        inline typename std::enable_if<!std::is_enum<T>::value, long>::
        type bitmask_cast(const T data)
        {
            return boost::numeric_cast<long>(data);
        }

        template <typename T>
        inline typename std::enable_if<std::is_enum<T>::value, long>::
        type bitmask_cast(const T data)
        {
            return bitmask_cast(
                static_cast<
                    typename std::underlying_type<T>::type
                >(data)
            );
        }

        inline long bitmask_merge() { return 0; }

        template <typename Arg, typename ... Args>
        inline long bitmask_merge(const Arg arg, Args &&...args)
        {
            return bitmask_cast(arg) |
                   bitmask_merge(std::forward<Args>(args)...);
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
        bitmask(Args &&...args):
            m_data(detail::bitmask_merge(
                std::forward<Args>(args)...
            )) {}

        inline long data() const
        {
            return m_data;
        }

    private:
        long m_data = detail::bitmask_cast(Default);
    };
}}}}