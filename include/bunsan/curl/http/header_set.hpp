#pragma once

#include <bunsan/curl/http/header.hpp>

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index_container.hpp>

namespace bunsan{namespace curl{namespace http
{
    class header_set
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
        using index_type = container_type::index<tag_hashed>::type;
        using iterator = index_type::iterator;
        using const_iterator = index_type::const_iterator;

    public:
        void merge_insert(const header &h);

        const index_type &index() const { return m_headers.get<tag_hashed>(); }
        index_type &index() { return m_headers.get<tag_hashed>(); }

        bool operator==(const header_set &h) const
        {
            return m_headers == h.m_headers;
        }

    private:
        container_type m_headers;
    };

    std::ostream &operator<<(std::ostream &out, const header_set &h);
}}}
