#pragma once

#include <bunsan/curl/detail/easy.hpp>
#include <bunsan/curl/detail/static_range.hpp>
#include <bunsan/curl/error.hpp>

#include <curl/curl.h>

#include <boost/range/iterator_range.hpp>
#include <boost/scope_exit.hpp>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <CURLoption DataId, CURLoption SizeId, typename Wrapper>
    class wrapped_bytes: public Wrapper
    {
    public:
        typedef typename Wrapper::copy_policy copy_policy;

        using Wrapper::Wrapper;

        constexpr CURLoption data_id() const { return DataId; }
        constexpr CURLoption size_id() const { return SizeId; }

        curl::detail::static_range<CURLoption> ids() const
        {
            return curl::detail::make_static_range<CURLoption, DataId, SizeId>();
        }

        void setopt(CURL *const curl) const
        {
            bool commit = false;
            BOOST_SCOPE_EXIT_ALL(curl, this, &commit)
            {
                if (!commit)
                {
                    // see Curl_init_userdefined()
                    constexpr decltype(Wrapper::size()) unknown_size = -1;

                    // noexcept
                    ::curl_easy_setopt(curl, size_id(), unknown_size);
                    ::curl_easy_setopt(curl, data_id(), nullptr);
                }
            };
            curl::detail::easy::setopt(curl, size_id(), Wrapper::size());
            curl::detail::easy::setopt(curl, data_id(), Wrapper::data());
            commit = true;
        }
    };
}}}}

#define BUNSAN_CURL_BYTES_WRAPPED(NAME, DATA, SIZE, ...) \
    typedef ::bunsan::curl::options::wrapper::wrapped_bytes< \
        DATA, SIZE, ::bunsan::curl::options::wrapper::__VA_ARGS__> NAME;
