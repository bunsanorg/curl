#pragma once

#include <bunsan/curl/http/header.hpp>
#include <bunsan/curl/http/status.hpp>

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index_container.hpp>

#include <ostream>
#include <string>
#include <unordered_map>

namespace bunsan{namespace curl{namespace http
{
    class response_head
    {
    private:
        using container_type = boost::multi_index_container<
            header,
            boost::multi_index::indexed_by<
                boost::multi_index::hashed_unique<
                    boost::multi_index::tag<struct tag_hashed>,
                    boost::multi_index::const_mem_fun<
                        header,
                        const std::string &,
                        &header::name
                    >
                >
            >
        >;

    public:
        using header_index = container_type::index<tag_hashed>::type;
        using header_iterator = header_index::iterator;
        using header_const_iterator = header_index::const_iterator;

    public:
        explicit response_head(const http::status &s);

        response_head(const response_head &)=default;
        response_head(response_head &&)=default;
        response_head &operator=(const response_head &)=default;
        response_head &operator=(response_head &&)=default;

        void append(const header &h);

        const http::status &status() const { return m_status; }
        const header_index &headers() const { return m_headers.get<tag_hashed>(); }
        header_index &headers() { return m_headers.get<tag_hashed>(); }

        bool operator==(const response_head &h) const
        {
            return m_status == h.m_status &&
                   m_headers == h.m_headers;
        }

    private:
        http::status m_status;
        container_type m_headers;
    };

    std::ostream &operator<<(std::ostream &out, const response_head &r);
}}}
