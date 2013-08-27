#pragma once

#include <curl/curl.h>

#include <boost/noncopyable.hpp>
#include <boost/range/any_range.hpp>

#include <array>
#include <memory>

namespace bunsan{namespace curl{namespace options
{
    namespace detail
    {
        class option_base: private boost::noncopyable
        {
        public:
            typedef std::unique_ptr<option_base> option_ptr;
            typedef std::shared_ptr<option_base> shared_option_ptr;

            typedef boost::any_range<
                const CURLoption,
                boost::forward_traversal_tag,
                const CURLoption &,
                std::ptrdiff_t> const_id_range;

        public:
            virtual void init(CURL *curl) const=0;

            virtual const_id_range ids() const=0;

            virtual option_ptr clone() const=0;

            virtual ~option_base();
        };

        typedef option_base::option_ptr option_ptr;
        typedef option_base::shared_option_ptr shared_option_ptr;
    }

    using detail::option_ptr;
    using detail::shared_option_ptr;

    template <typename T>
    class option: public detail::option_base
    {
    public:
        explicit option(const T &impl): m_impl(impl) {}

        void init(CURL *curl) const override
        {
            m_impl.init(curl);
        }

        const_id_range ids() const override
        {
            return m_impl.ids();
        }

        option_ptr clone() const override
        {
            option_ptr tmp(new option<T>(this->m_impl));
            return tmp;
        }

    private:
        T m_impl;
    };

    class option_set
    {
    public:
        option_set()=default;
        option_set(option_set &&)=default;
        option_set &operator=(option_set &&)=default;

        option_set(const option_set &);
        option_set &operator=(const option_set &);

        void swap(option_set &) noexcept;

        template <typename Option>
        void add(const Option &opt)
        {
            option_ptr tmp(new option<Option>(opt));
            add(std::move(tmp));
        };

        void add(option_ptr &&opt);

        void init(CURL *const curl) const;

        void clear();

    private:
        void reset(const CURLoption id);

    private:
        std::array<shared_option_ptr, CURLOPT_LASTENTRY> m_options;
    };

    inline void swap(option_set &a, option_set &b) noexcept
    {
        a.swap(b);
    }
}}}
