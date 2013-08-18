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
            return id;
        }
    }

    void option_set::add(detail::option_ptr &&opt)
    {
        BOOST_ASSERT(opt);
        const CURLoption id = curl_opt_id(opt->id());
        BOOST_ASSERT(0 <= id);
        BOOST_ASSERT(id < CURLOPT_LASTENTRY);
        m_options[id] = std::move(opt);
    }

    void option_set::init(CURL *const curl) const
    {
        for (const option_ptr &opt: m_options)
            if (opt)
                opt->init(curl);
    }

    void option_set::clear()
    {
        for (option_ptr &opt: m_options)
            opt.reset();
    }
}}}
