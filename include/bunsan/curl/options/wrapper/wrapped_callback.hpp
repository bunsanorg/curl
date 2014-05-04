#pragma once

#include <bunsan/curl/detail/easy.hpp>
#include <bunsan/curl/detail/static_range.hpp>
#include <bunsan/curl/error.hpp>
#include <bunsan/curl/options/traits.hpp>

#include <curl/curl.h>

#include <boost/scope_exit.hpp>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <CURLoption CallbackId, CURLoption DataId, typename Wrapper>
    class wrapped_callback: private Wrapper
    {
    public:
        typedef copy_policy::by_wrapper copy_policy;

        using Wrapper::Wrapper;

        constexpr CURLoption callback_id() const { return CallbackId; }
        constexpr CURLoption data_id() const { return DataId; }

        curl::detail::static_range<CURLoption> ids() const
        {
            return curl::detail::make_static_range<CURLoption, CallbackId, DataId>();
        }

        void setopt(CURL *const curl) const
        {
            bool commit = false;
            BOOST_SCOPE_EXIT_ALL(curl, this, &commit)
            {
                if (!commit)
                {
                    // noexcept
                    ::curl_easy_setopt(curl, callback_id(), nullptr);
                    ::curl_easy_setopt(curl, data_id(), nullptr);
                }
            };
            curl::detail::easy::setopt(curl, callback_id(), Wrapper::callback());
            curl::detail::easy::setopt(curl, data_id(), Wrapper::data());
            commit = true;
        }
    };
}}}}

#define BUNSAN_CURL_CALLBACK_WRAPPED(NAME, CALLBACK_ID, DATA_ID, ...) \
    typedef ::bunsan::curl::options::wrapper::wrapped_callback< \
        CALLBACK_ID, DATA_ID, ::bunsan::curl::options::wrapper::__VA_ARGS__> NAME;
