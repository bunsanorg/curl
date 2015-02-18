#include <bunsan/curl/http/header_set.hpp>

namespace bunsan{namespace curl{namespace http
{
    void header_set::append(const header &h)
    {
        const auto iter = index().find(h.name());
        if (iter != index().end())
            index().modify(iter, [&h](header &hdr) { hdr.merge(h); });
        else
            index().insert(h);
    }

    std::ostream &operator<<(std::ostream &out, const header_set &h)
    {
        for (const header &hdr: h.index())
            for (const std::string &str: hdr.headers())
                out << str << '\n';
        return out;
    }
}}}
