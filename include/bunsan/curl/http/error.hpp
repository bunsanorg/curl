#pragma once

#include <bunsan/curl/error.hpp>

namespace bunsan{namespace curl{namespace http
{
    struct error: virtual curl::error {};

    struct header_error: virtual error
    {
        using name = boost::error_info<struct tag_name, std::string>;
    };
    struct header_non_unique_value_error: virtual header_error {};
    struct header_parse_error: virtual header_error
    {
        using header_data = boost::error_info<struct tag_header, std::string>;
    };

    struct status_error: virtual header_error {};
    struct status_parse_error:
        virtual status_error,
        virtual header_parse_error {};
}}}
