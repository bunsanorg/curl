#pragma once

#include <bunsan/curl/error.hpp>

#include <curl/curl.h>

namespace bunsan{namespace curl{namespace options
{
    template <CURLoption Id, typename Wrapper>
    class wrapped_option: private Wrapper
    {
    public:
        typedef typename Wrapper::copy_policy copy_policy;

        using Wrapper::Wrapper;

        constexpr CURLoption id() const { return Id; }

        void init(CURL *const curl) const
        {
            const CURLcode ret = ::curl_easy_setopt(curl, id(), Wrapper::data());
            if (ret)
                BOOST_THROW_EXCEPTION(easy_error(ret, "curl_easy_setopt"));
        }
    };
}}}
