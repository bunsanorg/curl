#pragma once

#include <bunsan/curl/options/wrapped_option_helper.hpp>
#include <bunsan/curl/options/wrapper/bool.hpp>

namespace bunsan{namespace curl{namespace options
{
    BUNSAN_CURL_OPTION_WRAPPED(verbose, CURLOPT_VERBOSE, bool_01)
    BUNSAN_CURL_OPTION_WRAPPED(header, CURLOPT_HEADER, bool_01)
    BUNSAN_CURL_OPTION_WRAPPED(noprogress, CURLOPT_NOPROGRESS, bool_01)
    BUNSAN_CURL_OPTION_WRAPPED(nosignal, CURLOPT_NOSIGNAL, bool_01)
    BUNSAN_CURL_OPTION_WRAPPED(wildcardmatch, CURLOPT_WILDCARDMATCH, bool_01)
}}}
