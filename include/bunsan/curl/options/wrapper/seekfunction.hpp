#pragma once

#include <bunsan/curl/options/traits.hpp>
#include <bunsan/curl/options/wrapper/basic_function.hpp>
#include <bunsan/curl/seekfunc.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    struct seekfunction_traits
    {
        using wrapper_type = basic_function<seekfunction_traits>;

        using function_type = std::function<
            seekfunc (curl_off_t offset, int origin)
        >;

        static inline int static_call(void *instream, curl_off_t offset, int origin)
        {
            const auto this_ = static_cast<const wrapper_type *>(instream);
            return static_cast<int>(this_->call(offset, origin));
        }
    };

    using seekfunction = seekfunction_traits::wrapper_type;
}}}}
