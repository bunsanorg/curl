#pragma once

#include <curl/curl.h>

#include <memory>

namespace bunsan{namespace curl{namespace detail
{
    namespace slist
    {
        struct deleter
        {
            constexpr deleter() noexcept=default;

            void operator()(void *const ptr) const;
        };

        typedef std::unique_ptr<struct ::curl_slist, deleter> ptr;

        void append(ptr &list, const char *const data);

        inline void append(ptr &list, const std::string &data)
        {
            append(list, data.c_str());
        }
    }

    typedef slist::ptr slist_ptr;
}}}
