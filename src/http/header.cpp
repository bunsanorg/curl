#include <bunsan/curl/http/header.hpp>

#include <bunsan/curl/http/error.hpp>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/assert.hpp>
#include <boost/io/detail/quoted_manip.hpp>
#include <boost/regex.hpp>

namespace bunsan{namespace curl{namespace http
{
    const std::string &header::value() const
    {
        if (!unique_value())
            BOOST_THROW_EXCEPTION(
                header_non_unique_value_error() <<
                header_non_unique_value_error::name(name()));
        return m_values[0];
    }

    header::values_const_range header::values() const
    {
        return values_const_range(
            m_values.begin(),
            m_values.end()
        );
    }

    std::string header::make_name(const std::string &raw_name)
    {
        return boost::algorithm::to_lower_copy(raw_name);
    }

    header header::parse(const std::string &data)
    {
        static const boost::regex general_header(
            "^(?<name>[^ ]+)"
            " *: *"
            "(?<value>([^ ](.*[^ ])?)?)"
            " *$"
        );
        boost::smatch result;
        if (boost::regex_match(data, result, general_header))
        {
            const boost::ssub_match name = result["name"];
            const boost::ssub_match value = result["value"];
            BOOST_ASSERT(name.matched);
            BOOST_ASSERT(value.matched);
            return header{make_name(name), value};
        }
        BOOST_THROW_EXCEPTION(
            header_parse_error() <<
            header_parse_error::message("Not a header") <<
            header_parse_error::header_data(data));
    }

    std::ostream &operator<<(std::ostream &out, const header &h)
    {
        out << h.name() << ": ";
        if (h.unique_value())
        {
            out << h.value();
        }
        else
        {
            out << '[';
            bool first = true;
            for (const std::string &value: h.values())
            {
                if (!first)
                    out << ", ";
                first = false;
                out << boost::io::quoted(value);
            }
            out << ']';
        }
        return out;
    }
}}}
