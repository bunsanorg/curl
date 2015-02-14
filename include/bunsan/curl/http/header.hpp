#pragma once

#include <boost/range/iterator_range.hpp>

#include <initializer_list>
#include <ostream>
#include <string>
#include <vector>

namespace bunsan{namespace curl{namespace http
{
    class header
    {
    private:
        using values_type = std::vector<std::string>;

    public:
        using values_const_iterator = values_type::const_iterator;
        using values_const_range = boost::iterator_range<values_const_iterator>;

    public:
        header()=default;
        header(const header &)=default;
        header(header &&)=default;
        header &operator=(const header &)=default;
        header &operator=(header &&)=default;

        template <typename Arg, typename ... Args>
        header(const std::string &name, Arg &&arg, Args &&...args):
            m_name(make_name(name)),
            m_values({
                std::forward<Arg>(arg),
                std::forward<Args>(args)...
            }) {}

        bool operator==(const header &h) const
        {
            return m_name == h.m_name && m_values == h.m_values;
        }

        const std::string &name() const { return m_name; }

        bool unique_value() const { return m_values.size() == 1; }

        /// \throws header_non_unique_value_error if !unique_value()
        const std::string &value() const;

        values_const_range values() const;

        static std::string make_name(const std::string &raw_name);

        /// Parse header without trailing CRLF
        static header parse(const std::string &data);

    private:
        std::string m_name;
        std::vector<std::string> m_values;
    };

    std::ostream &operator<<(std::ostream &out, const header &h);
}}}
