#pragma once

#include <bunsan/curl/options/traits.hpp>

#include <curl/curl.h>

#include <boost/cast.hpp>

#include <chrono>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <typename Duration>
    class duration;

    template <typename Rep, typename Period>
    class duration<std::chrono::duration<Rep, Period>>
    {
    public:
        typedef copy_policy::by_curl copy_policy;

        typedef std::chrono::duration<Rep, Period> duration_type;

        template <typename Rep_, typename Period_>
        explicit duration(const std::chrono::duration<Rep_, Period_> &duration):
            m_data(std::chrono::duration_cast<duration_type>(duration)) {}

        explicit duration(const duration_type &duration): m_data(duration) {}

        inline long data() const
        {
            return boost::numeric_cast<long>(m_data.count());
        }

    private:
        duration_type m_data;
    };

    typedef duration<std::chrono::seconds> seconds;
    typedef duration<std::chrono::milliseconds> milliseconds;
}}}}
