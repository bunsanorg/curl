#pragma once

#include <bunsan/curl/options/traits.hpp>

#include <curl/curl.h>

#include <functional>
#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    class writefunction
    {
    public:
        template <typename Arg, typename ... Args>
        explicit writefunction(Arg &&arg, Args &&...args):
            m_callback(
                std::forward<Arg>(arg),
                std::forward<Args>(args)...
            ) {}

        curl_write_callback callback() const
        {
            return &writefunction::call;
        }

        void *data() const
        {
            // cURL's interface requires (void *)
            return const_cast<writefunction *>(this);
        }

    private:
        static std::size_t call(
            char *ptr, size_t size, size_t nmemb, void *userdata)
        {
            const auto this_ = static_cast<const writefunction *>(userdata);
            return this_->call_(ptr, size, nmemb);
        }

        std::size_t call_(char *ptr, size_t size, size_t nmemb) const
        {
            return m_callback(ptr, size, nmemb);
        }

    private:
        std::function<
            std::size_t (char *ptr, size_t size, size_t nmemb)
        > m_callback;
    };
}}}}
