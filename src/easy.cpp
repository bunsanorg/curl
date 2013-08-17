#include <bunsan/curl/easy.hpp>

#include <bunsan/curl/error.hpp>

#include <curl/curl.h>

#include <boost/assert.hpp>

namespace bunsan{namespace curl
{
    easy::easy(std::nullptr_t) noexcept: m_curl(nullptr) {}

    easy &easy::operator=(std::nullptr_t) noexcept
    {
        if (m_curl)
            ::curl_easy_cleanup(m_curl);
        m_curl = nullptr;
        return *this;
    }

    easy::easy(CURL *const curl) noexcept: m_curl(curl)
    {
        BOOST_ASSERT_MSG(!get_(curl), "CURL is already captured.");
        init();
    }

    easy::easy(easy &&curl) noexcept:
        m_curl(curl.m_curl)
    {
        curl.m_curl = nullptr;
        init();
    }

    easy &easy::operator=(easy &&curl) noexcept
    {
        m_curl = curl.m_curl;
        curl.m_curl = nullptr;
        init();
        return *this;
    }

    easy::easy(): m_curl(::curl_easy_init())
    {
        if (!m_curl)
            BOOST_THROW_EXCEPTION(easy_error() <<
                                  easy_error::what_message("curl_easy_init"));
    }

    easy::~easy()
    {
        *this = nullptr;
    }

    void easy::swap(easy &curl) noexcept
    {
        using std::swap;

        swap(m_curl, curl.m_curl);
        init();
        curl.init();
    }

    easy::operator bool() const noexcept
    {
        return m_curl;
    }

    void easy::perform()
    {
        BOOST_ASSERT(*this);

        const CURLcode ret = ::curl_easy_perform(m_curl);
        if (ret)
            BOOST_THROW_EXCEPTION(easy_error(ret, "curl_easy_perform"));
    }

    void easy::pause(const int bitmask)
    {
        BOOST_ASSERT(*this);

        const CURLcode ret = ::curl_easy_pause(m_curl, bitmask);
        if (ret)
            BOOST_THROW_EXCEPTION(easy_error(ret, "curl_easy_pause"));
    }

    void easy::init() noexcept
    {
        if (*this)
        {
            const CURLcode ret = ::curl_easy_setopt(m_curl, CURLOPT_PRIVATE, this);
            BOOST_ASSERT_MSG(ret == CURLE_OK, easy_category().message(ret).c_str());
        }
    }

    easy &easy::get(CURL *const curl) noexcept
    {
        easy *const obj = get_(curl);
        BOOST_ASSERT_MSG(obj, "CURL was not captured.");
        BOOST_ASSERT(obj->m_curl == curl);
        return *obj;
    }

    easy *easy::get_(CURL *const curl) noexcept
    {
        BOOST_ASSERT(curl);

        char *obj;
        const CURLcode ret = ::curl_easy_getinfo(curl, CURLINFO_PRIVATE, &obj);
        BOOST_ASSERT_MSG(ret == CURLE_OK, easy_category().message(ret).c_str());
        return reinterpret_cast<easy *>(obj);
    }
}}
