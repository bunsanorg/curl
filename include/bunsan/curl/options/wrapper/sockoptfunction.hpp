#pragma once

#include <bunsan/curl/options/traits.hpp>
#include <bunsan/curl/options/wrapper/basic_function.hpp>
#include <bunsan/curl/socktype.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    struct sockoptfunction_traits
    {
        using wrapper = basic_function<sockoptfunction_traits>;

        using function_type = std::function<
            int (curl_socket_t curlfd, curl::socktype purpose)
        >;

        static inline int static_call(
            void *clientp, curl_socket_t curlfd, curlsocktype purpose)
        {
            const auto this_ = static_cast<const wrapper *>(clientp);
            return this_->call(curlfd, static_cast<curl::socktype>(purpose));
        }
    };

    using sockoptfunction = sockoptfunction_traits::wrapper;
}}}}
