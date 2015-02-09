#pragma once

#include <bunsan/curl/options/traits.hpp>
#include <bunsan/curl/options/wrapper/basic_function.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    struct headerfunction_traits
    {
        using wrapper = basic_function<headerfunction_traits>;

        using function_type = std::function<
            std::size_t (char *ptr, std::size_t size)
        >;

        static inline std::size_t static_call(
            void *ptr, std::size_t size, std::size_t nmemb, void *userdata)
        {
            const auto this_ = static_cast<const wrapper *>(userdata);
            return this_->call(static_cast<char *>(ptr), size * nmemb);
        }
    };

    using headerfunction = headerfunction_traits::wrapper;
}}}}
