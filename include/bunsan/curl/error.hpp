#pragma once

#include <bunsan/curl/error_category.hpp>

#include <bunsan/categorized_error.hpp>

#include <curl/curl.h>

namespace bunsan{namespace curl
{
    struct error: virtual bunsan::error {};

    template <typename Code, const std::error_category &(*GetCategory)()>
    struct basic_error: virtual error, virtual categorized_error
    {
        basic_error()=default;

        explicit basic_error(const Code code):
            basic_error(std::error_code(code, GetCategory())) {}

        basic_error(const Code code, const std::string &what):
            basic_error(std::error_code(code, GetCategory()), what) {}

        basic_error(const std::error_code &ec):
            categorized_error(ec) {}

        basic_error(const std::error_code &ec, const std::string &what):
            categorized_error(ec, what) {}
    };

    struct easy_error: basic_error<CURLcode, &easy_category>
    {
        using basic_error::basic_error;
    };

    struct multi_error: basic_error<CURLcode, &multi_category>
    {
        using basic_error::basic_error;
    };
}}
