#pragma once

#include <bunsan/curl/detail/string_list.hpp>
#include <bunsan/curl/options/traits.hpp>

#include <curl/curl.h>

#include <initializer_list>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    class string_list
    {
    public:
        typedef copy_policy::by_wrapper copy_policy;

        string_list()=default;

        template <typename Container>
        explicit string_list(const Container &container):
            m_data(begin(container), end(container)) {}

        template <typename T>
        explicit string_list(const std::initializer_list<T> &container):
            m_data(begin(container), end(container)) {}

        inline const ::curl_slist *data() const
        {
            return m_data.data();
        }

    private:
        curl::detail::string_list m_data;
    };
}}}}
