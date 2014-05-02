#pragma once

#include <bunsan/curl/options/traits.hpp>

#include <bunsan/optional.hpp>

#include <curl/curl.h>

#include <string>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    class string
    {
    public:
        typedef copy_policy::by_curl copy_policy;

        string(none_t): m_data(none) {}

        template <typename Arg, typename ... Args>
        string(Arg &&arg, Args &&...args):
            m_data(std::string(
                std::forward<Arg>(arg),
                std::forward<Args>(args)...
            )) {}

        inline const char *data() const
        {
            if (m_data)
                return m_data->c_str();
            else
                return nullptr;
        }

    private:
        optional<std::string> m_data;
    };
}}}}
