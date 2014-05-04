#pragma once

#include <bunsan/curl/easy.hpp>
#include <bunsan/curl/ioerr.hpp>
#include <bunsan/curl/options/traits.hpp>
#include <bunsan/curl/options/wrapper/basic_function.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    struct ioctlfunction_traits
    {
        typedef basic_function<ioctlfunction_traits> wrapper;

        typedef std::function<
            curl::ioerr (curl::easy &, int cmd)
        > function_type;

        static inline curlioerr static_call(CURL *handle, int cmd, void *clientp)
        {
            const auto this_ = static_cast<const wrapper *>(clientp);
            return static_cast<curlioerr>(this_->call(curl::easy::get(handle), cmd));
        }
    };

    typedef ioctlfunction_traits::wrapper ioctlfunction;
}}}}
