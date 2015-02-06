#pragma once

#include <bunsan/curl/detail/slist.hpp>

#include <boost/iterator/iterator_facade.hpp>

#include <algorithm>
#include <utility>

namespace bunsan{namespace curl{namespace detail
{
    class string_list_iterator:
        public boost::iterator_facade<
            string_list_iterator, const char * const, boost::forward_traversal_tag>
    {
    public:
        /// End iterator
        string_list_iterator()=default;

        string_list_iterator(const string_list_iterator &)=default;
        string_list_iterator &operator=(const string_list_iterator &)=default;

        explicit string_list_iterator(const ::curl_slist *const arg):
            m_value(arg) {}

    private:
        friend class boost::iterator_core_access;

        void increment()
        {
            m_value = m_value->next;
        }

        bool equal(const string_list_iterator &other) const
        {
            return m_value == other.m_value;
        }

        value_type dereference() const { return m_value->data; }

    private:
        const ::curl_slist *m_value = nullptr;
    };

    class string_list
    {
    public:
        typedef const char *value_type;
        typedef string_list_iterator iterator;
        typedef string_list_iterator const_iterator;

    public:
        string_list()=default;

        string_list(string_list &&)=default;
        string_list &operator=(string_list &&)=default;

        string_list(const string_list &);
        string_list &operator=(const string_list &);

        template <typename Iterator>
        explicit string_list(Iterator begin, const Iterator end)
        {
            for (; begin != end; ++begin)
                append(*begin);
        }

        template <typename Arg, typename ... Args>
        void assign(Arg &&arg, Args &&...args)
        {
            string_list(
                std::forward<Arg>(arg),
                std::forward<Args>(args)...
            ).swap(*this);
        }

        void clear()
        {
            m_data.reset();
        }

        inline void swap(string_list &list) noexcept
        {
            m_data.swap(list.m_data);
        }

        template <typename T>
        void append(T &&arg)
        {
            slist::append(m_data, std::forward<T>(arg));
        }

        inline const_iterator begin() const { return const_iterator(m_data.get()); }
        inline const_iterator end() const { return const_iterator(); }
        inline const_iterator cbegin() const { return begin(); }
        inline const_iterator cend() const { return end(); }

        inline bool empty() const { return !m_data; }
        inline std::size_t size() const { return std::distance(begin(), end()); }

        inline ::curl_slist *data()
        {
            return m_data.get();
        }

        inline const ::curl_slist *data() const
        {
            return m_data.get();
        }

    private:
        slist_ptr m_data;
    };

    inline void swap(string_list &a, string_list &b) noexcept
    {
        a.swap(b);
    }

    bool operator==(const string_list &a, const string_list &b);

    inline string_list::const_iterator begin(const string_list &list)
    {
        return list.begin();
    }

    inline string_list::const_iterator end(const string_list &list)
    {
        return list.end();
    }
}}}
