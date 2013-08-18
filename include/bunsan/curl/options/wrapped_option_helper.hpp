#pragma once

#include <bunsan/curl/options/wrapped_option.hpp>

#include <curl/curl.h>

#define BUNSAN_CURL_OPTION_WRAPPED(NAME, CODE, WRAPPER) \
    typedef ::bunsan::curl::options::wrapped_option<\
        CODE, ::bunsan::curl::options::wrapper::WRAPPER> NAME;
