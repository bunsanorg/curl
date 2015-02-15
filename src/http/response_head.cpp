#include <bunsan/curl/http/response_head.hpp>

namespace bunsan{namespace curl{namespace http
{
    response_head::response_head(const http::status &s):
        m_status(s) {}

    void response_head::append(const header &h)
    {
        const auto iter = headers().find(h.name());
        if (iter != headers().end())
            headers().modify(iter, [&h](header &hdr) { hdr.merge(h); });
        else
            headers().insert(h);
    }

    std::ostream &operator<<(std::ostream &out, const response_head &r)
    {
        out << r.status() << "\n";
        for (const header &h: r.headers())
            out << h << '\n';
        return out;
    }
}}}
