#pragma once

#include <ostream>
#include <string>

namespace bunsan{namespace curl{namespace http
{
    struct header
    {
        std::string name, value;

        header()=default;
        header(const header &)=default;
        header(header &&)=default;
        header &operator=(const header &)=default;
        header &operator=(header &&)=default;

        header(const std::string &name_, const std::string &value_):
            name(name_), value(value_) {}

        bool operator==(const header &h) const
        {
            return name == h.name && value == h.value;
        }

        static std::string make_name(const std::string &name);

        /// Parse header without trailing CRLF
        static header parse(const std::string &data);
    };

    std::ostream &operator<<(std::ostream &out, const header &h);
}}}
