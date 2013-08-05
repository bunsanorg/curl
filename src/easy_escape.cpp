#include <bunsan/curl/easy.hpp>

#include <bunsan/curl/detail/curl_delete.hpp>
#include <bunsan/curl/error.hpp>

#include <memory>

namespace bunsan{namespace curl
{
    std::string easy::escape(const std::string &url)
    {
        if (url.empty())
        {
            return std::string();
        }
        else
        {
            const std::unique_ptr<char, detail::curl_delete> ptr(::curl_easy_escape(m_curl, url.data(), url.size()));
            if (!ptr)
                BOOST_THROW_EXCEPTION(easy_error() <<
                                      easy_error::what_message("curl_easy_escape"));
            return std::string(ptr.get());
        }
    }

    std::string easy::unescape(const std::string &url)
    {
        if (url.empty())
        {
            return std::string();
        }
        else
        {
            int size;
            const std::unique_ptr<char, detail::curl_delete> ptr(::curl_easy_unescape(m_curl, url.data(), url.size(), &size));
            if (!ptr)
                BOOST_THROW_EXCEPTION(easy_error() <<
                                      easy_error::what_message("curl_easy_unescape"));
            return std::string(ptr.get(), size);
        }
    }
}}
