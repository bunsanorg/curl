#include <bunsan/curl/http/header.hpp>

#include <bunsan/curl/http/error.hpp>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/assert.hpp>
#include <boost/regex.hpp>

namespace bunsan{namespace curl{namespace http
{
    std::string header::make_name(const std::string &name)
    {
        return boost::algorithm::to_lower_copy(name);
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
            header_parse_error::header(data));
    }

    std::ostream &operator<<(std::ostream &out, const header &h)
    {
        return out << h.name << ": " << h.value;
    }
}}}
