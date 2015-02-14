#pragma once

#include <bunsan/curl/error.hpp>

namespace bunsan{namespace curl{namespace http
{
    struct error: virtual curl::error {};

    struct header_error: virtual error
    {
        typedef boost::error_info<struct tag_name, std::string> name;
    };
    struct header_non_unique_value_error: virtual header_error {};
    struct header_parse_error: virtual header_error
    {
        typedef boost::error_info<struct tag_header, std::string> header_data;
    };

    struct status_error: virtual header_error {};
    struct status_parse_error:
        virtual status_error,
        virtual header_parse_error {};
}}}
