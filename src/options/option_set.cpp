#include <bunsan/curl/options/option_set.hpp>

#include <boost/assert.hpp>

namespace bunsan{namespace curl{namespace options
{
    namespace detail
    {
        option_base::~option_base() {}
    }

    option_set::option_set(const option_set &set)
    {
        for (std::size_t i = 0; i < m_options.size(); ++i)
            if (set.m_options[i])
                m_options[i] = set.m_options[i]->clone();
    }

    option_set &option_set::operator=(const option_set &set)
    {
        option_set(set).swap(*this);
        return *this;
    }

    void option_set::swap(option_set &set) noexcept
    {
        using std::swap;

        swap(m_options, set.m_options);
    }

    namespace
    {
        CURLoption curl_opt_id(CURLoption id)
        {
            constexpr CURLoption type = static_cast<CURLoption>(10000);
            while (id >= type)
                id = static_cast<CURLoption>(id - type);
            BOOST_ASSERT(0 <= id);
            BOOST_ASSERT(id < CURLOPT_LASTENTRY);
            return id;
        }
    }

    void option_set::add(option_ptr &&opt)
    {
        BOOST_ASSERT(opt);
        shared_option_ptr opt_(opt.release());
        const detail::option_base::const_id_range range = opt_->ids();
        BOOST_ASSERT(!range.empty());
        for (const CURLoption &id: range)
            reset(id);
        for (const CURLoption &id: range)
            m_options[curl_opt_id(id)] = opt_;
    }

    void option_set::add_and_setopt(option_ptr &&opt, CURL *const curl)
    {
        const detail::option_base::const_id_range range = opt->ids();
        BOOST_ASSERT(!range.empty());
        const CURLoption id = *range.begin();
        const CURLoption id_ = curl_opt_id(id);
        add(std::move(opt));
        BOOST_ASSERT(m_options[id_]);
        m_options[id_]->setopt(curl);
    }

    void option_set::setopt(CURL *const curl) const
    {
        for (const shared_option_ptr &opt: m_options)
            if (opt)
                opt->setopt(curl);
    }

    void option_set::clear()
    {
        for (shared_option_ptr &opt: m_options)
            opt.reset();
    }

    void option_set::reset(const CURLoption id)
    {
        const CURLoption id_ = curl_opt_id(id);
        const shared_option_ptr opt = m_options[id_];
        if (opt)
        {
            for (const CURLoption &other_id: opt->ids())
            {
                const CURLoption other_id_ = curl_opt_id(other_id);
                BOOST_ASSERT(m_options[other_id_]);
                BOOST_ASSERT(m_options[other_id_] == opt);
                m_options[other_id_].reset();
            }
        }
        BOOST_ASSERT(!m_options[id_]);
    }
}}}
