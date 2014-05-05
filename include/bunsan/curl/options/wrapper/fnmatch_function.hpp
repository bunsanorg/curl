#pragma once

#include <bunsan/curl/fnmatch.hpp>
#include <bunsan/curl/options/traits.hpp>
#include <bunsan/curl/options/wrapper/basic_function.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    struct fnmatch_function_traits
    {
        typedef basic_function<fnmatch_function_traits> wrapper;

        typedef std::function<
            curl::fnmatch (const char *pattern, const char *string)
        > function_type;

        static inline int static_call(
            void *ptr, const char *pattern, const char *string)
        {
            const auto this_ = static_cast<const wrapper *>(ptr);
            return static_cast<int>(this_->call(pattern, string));
        }
    };

    typedef fnmatch_function_traits::wrapper fnmatch_function;
}}}}
