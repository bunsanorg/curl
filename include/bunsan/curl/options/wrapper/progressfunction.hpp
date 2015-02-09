#pragma once

#include <bunsan/curl/options/traits.hpp>
#include <bunsan/curl/options/wrapper/basic_function.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <typename T>
    struct basic_progressfunction_traits
    {
        using wrapper = basic_function<basic_progressfunction_traits<T>>;

        using function_type = std::function<
            int (T dltotal, T dlnow, T ultotal, T ulnow)
        >;

        static inline int static_call(
            void *clientp, T dltotal, T dlnow, T ultotal, T ulnow)
        {
            const auto this_ = static_cast<const wrapper *>(clientp);
            return this_->call(dltotal, dlnow, ultotal, ulnow);
        }
    };

    template <typename T>
    using basic_progressfunction = typename basic_progressfunction_traits<T>::wrapper;

    using progressfunction = basic_progressfunction<double>;
    using xferinfofunction = basic_progressfunction<curl_off_t>;
}}}}
