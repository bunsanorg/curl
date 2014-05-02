#pragma once

#include <bunsan/curl/options/wrapper/wrapped_option.hpp>

#include <curl/curl.h>

#define BUNSAN_CURL_OPTION_WRAPPED(NAME, CODE, WRAPPER) \
    typedef ::bunsan::curl::options::wrapper::wrapped_option<\
        CODE, ::bunsan::curl::options::wrapper::WRAPPER> NAME;
