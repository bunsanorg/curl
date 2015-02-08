#pragma once

#include <bunsan/curl/detail/slist.hpp>

#include <curl/curl.h>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

namespace bunsan{namespace curl{namespace detail
{
    class slist_iterator:
        public boost::iterator_facade<
            slist_iterator,
            const char * const,
            boost::forward_traversal_tag,
            const char * const
        >
    {
    public:
        /// End iterator
        slist_iterator()=default;

        slist_iterator(const slist_iterator &)=default;
        slist_iterator &operator=(const slist_iterator &)=default;

        explicit slist_iterator(const ::curl_slist *const arg):
            m_value(arg) {}

    private:
        friend class boost::iterator_core_access;

        void increment()
        {
            m_value = m_value->next;
        }

        bool equal(const slist_iterator &other) const
        {
            return m_value == other.m_value;
        }

        value_type dereference() const { return m_value->data; }

    private:
        const ::curl_slist *m_value = nullptr;
    };

    using slist_range = boost::iterator_range<slist_iterator>;

    inline slist_range make_slist_range(const ::curl_slist *const list)
    {
        return boost::make_iterator_range(
            slist_iterator(list),
            slist_iterator()
        );
    }

    inline slist_range make_slist_range(const slist_ptr &list)
    {
        return make_slist_range(list.get());
    }
}}}
