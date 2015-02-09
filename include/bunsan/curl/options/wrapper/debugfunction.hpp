#pragma once

#include <bunsan/curl/easy.hpp>
#include <bunsan/curl/infotype.hpp>
#include <bunsan/curl/options/traits.hpp>
#include <bunsan/curl/options/wrapper/basic_function.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    struct debugfunction_traits
    {
        using wrapper = basic_function<debugfunction_traits>;

        using function_type = std::function<
            void (curl::easy &, curl::infotype, char *, std::size_t)
        >;

        static inline int static_call(
            CURL *handle,
            curl_infotype info,
            char *ptr,
            std::size_t size,
            void *userdata)
        {
            const auto this_ = static_cast<const wrapper *>(userdata);
            this_->call(
                curl::easy::get(handle),
                static_cast<curl::infotype>(info),
                ptr,
                size
            );
            return 0;
        }
    };

    using debugfunction = debugfunction_traits::wrapper;
}}}}
