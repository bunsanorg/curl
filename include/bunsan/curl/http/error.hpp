#pragma once

#include <bunsan/curl/error.hpp>

namespace bunsan{namespace curl{namespace http
{
    struct error: virtual curl::error {};

    struct header_parse_error: virtual error
    {
        typedef boost::error_info<struct tag_header, std::string> header;
    };

    struct status_parse_error: virtual header_parse_error {};
}}}
