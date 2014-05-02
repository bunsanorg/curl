#pragma once

#include <bunsan/curl/detail/one_step_iterator.hpp>

#include <bunsan/curl/detail/easy.hpp>
#include <bunsan/curl/error.hpp>

#include <curl/curl.h>

#include <boost/range/iterator_range.hpp>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <CURLoption Id, typename Wrapper>
    class wrapped_option: private Wrapper
    {
    public:
        typedef typename Wrapper::copy_policy copy_policy;

        using Wrapper::Wrapper;

        constexpr CURLoption id() const { return Id; }

        auto ids() const ->
            decltype(detail::make_one_step_range<const CURLoption>(this->id()))
        {
            return detail::make_one_step_range<const CURLoption>(id());
        }

        void setopt(CURL *const curl) const
        {
            detail::easy::setopt(curl, id(), Wrapper::data());
        }
    };
}}}}
