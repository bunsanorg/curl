#pragma once

#include <bunsan/curl/options/wrapper/string.hpp>

#include <curl/curl.h>

#include <initializer_list>
#include <sstream>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    namespace detail
    {
        /// \todo verify container items
        template <char Separator, typename Container>
        std::string csv_list_string(const Container &container)
        {
            std::ostringstream sout;

            bool first = true;

            for (const auto &obj: container)
            {
                if (first)
                    first = false;
                else
                    sout << Separator;

                sout << obj;
            }

            return sout.str();
        }
    }

    template <char Separator=','>
    class csv_list: public string
    {
    public:
        csv_list()=default;

        template <typename Container>
        explicit csv_list(const Container &container):
            string(detail::csv_list_string<Separator>(container)) {}

        template <typename T>
        explicit csv_list(const std::initializer_list<T> &container):
            string(detail::csv_list_string<Separator>(container)) {}
    };
}}}}
