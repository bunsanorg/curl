#pragma once

#include <bunsan/curl/easy.hpp>
#include <bunsan/curl/options/traits.hpp>
#include <bunsan/curl/options/wrapper/basic_function.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    struct ssl_ctx_function_traits
    {
        typedef basic_function<ssl_ctx_function_traits> wrapper;

        typedef std::function<
            CURLcode (curl::easy &, void *sslctx)
        > function_type;

        static inline CURLcode static_call(
            CURL *handle, void *sslctx, void *parm)
        {
            const auto this_ = static_cast<const wrapper *>(parm);
            return this_->call(curl::easy::get(handle), sslctx);
        }
    };

    typedef ssl_ctx_function_traits::wrapper ssl_ctx_function;
}}}}
