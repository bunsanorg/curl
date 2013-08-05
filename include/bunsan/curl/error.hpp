#pragma once

#include <bunsan/categorized_error.hpp>

#include <curl/curl.h>

namespace bunsan{namespace curl
{
    struct error: virtual bunsan::error {};

    struct easy_error: virtual error, virtual categorized_error
    {
        easy_error()=default;
        explicit easy_error(const CURLcode code);
        easy_error(const CURLcode code, const std::string &what);

        // inherited
        explicit easy_error(const std::error_code &ec);
        easy_error(const std::error_code &ec, const std::string &what);
    };

    struct multi_error: virtual error, virtual categorized_error
    {
        multi_error()=default;
        explicit multi_error(const CURLcode code);
        multi_error(const CURLcode code, const std::string &what);

        // inherited
        explicit multi_error(const std::error_code &ec);
        multi_error(const std::error_code &ec, const std::string &what);
    };
}}
