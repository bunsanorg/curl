#pragma once

#include <bunsan/curl/options/traits.hpp>
#include <bunsan/curl/options/wrapper/basic_function.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    struct closesocketfunction_traits
    {
        using wrapper = basic_function<closesocketfunction_traits>;

        using function_type = std::function<
            int (curl_socket_t item)
        >;

        static inline int static_call(
            void *clientp, curl_socket_t item)
        {
            const auto this_ = static_cast<const wrapper *>(clientp);
            return this_->call(item);
        }
    };

    using closesocketfunction = closesocketfunction_traits::wrapper;
}}}}
