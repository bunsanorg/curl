#pragma once

#include <bunsan/curl/seekfunc.hpp>
#include <bunsan/curl/options/traits.hpp>
#include <bunsan/curl/options/wrapper/basic_function.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    struct seekfunction_traits
    {
        typedef basic_function<seekfunction_traits> wrapper;

        typedef std::function<
            seekfunc (curl_off_t offset, int origin)
        > function_type;

        static inline int static_call(void *instream, curl_off_t offset, int origin)
        {
            const auto this_ = static_cast<const wrapper *>(instream);
            return static_cast<int>(this_->call(offset, origin));
        }
    };

    typedef seekfunction_traits::wrapper seekfunction;
}}}}
