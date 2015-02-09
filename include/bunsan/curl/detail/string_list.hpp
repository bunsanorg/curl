#pragma once

#include <bunsan/curl/detail/slist.hpp>
#include <bunsan/curl/detail/slist_iterator.hpp>

#include <algorithm>
#include <utility>

namespace bunsan{namespace curl{namespace detail
{
    class string_list
    {
    public:
        using value_type = const char *;
        using iterator = slist_iterator;
        using const_iterator = slist_iterator;

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

        inline const_iterator begin() const { return const_iterator(data()); }
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
