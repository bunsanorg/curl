#pragma once

#include <bunsan/curl/detail/easy.hpp>
#include <bunsan/curl/detail/one_step_iterator.hpp>
#include <bunsan/curl/error.hpp>

#include <curl/curl.h>

#include <boost/range/iterator_range.hpp>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <CURLoption Id, typename Wrapper>
    class wrapped_option: private Wrapper
    {
    public:
        using copy_policy = typename Wrapper::copy_policy;

        using Wrapper::Wrapper;

        constexpr CURLoption id() const { return Id; }

        curl::detail::one_step_range<const CURLoption> ids() const
        {
            return curl::detail::make_one_step_range<const CURLoption>(id());
        }

        void setopt(CURL *const curl) const
        {
            curl::detail::easy::setopt(curl, id(), Wrapper::data());
        }
    };
}}}}

#define BUNSAN_CURL_OPTION_WRAPPED(NAME, CODE, ...) \
    using NAME = ::bunsan::curl::options::wrapper::wrapped_option< \
        CODE, ::bunsan::curl::options::wrapper::__VA_ARGS__>;
