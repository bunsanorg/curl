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
}}
