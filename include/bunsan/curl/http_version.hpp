#pragma once

#include <curl/curl.h>

#include <bunsan/stream_enum.hpp>

namespace bunsan{namespace curl
{
    BUNSAN_TYPED_STREAM_ENUM_CLASS_INITIALIZED(http_version, long,
    (
        (http_none, CURL_HTTP_VERSION_NONE),
        (http_1_0, CURL_HTTP_VERSION_1_0),
        (http_1_1, CURL_HTTP_VERSION_1_1),
        (http_2_0, CURL_HTTP_VERSION_2_0)
    ))
    static_assert(
        static_cast<long>(http_version::http_2_0) + 1 == CURL_HTTP_VERSION_LAST,
        "outdated"
    );

    struct http_version_unsupported_error: virtual http_version_invalid_value_error
    {
        typedef boost::error_info<struct tag_version_major, unsigned> version_major;
        typedef boost::error_info<struct tag_version_minor, unsigned> version_minor;
    };

    http_version make_http_version(const unsigned version_major,
                                   const unsigned version_minor);
}}
