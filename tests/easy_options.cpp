#define BOOST_TEST_MODULE easy_options
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/easy.hpp>
#include <bunsan/curl/options/behavior.hpp>
#include <bunsan/curl/options/callback.hpp>
#include <bunsan/curl/options/network.hpp>

#include </usr/include/boost/algorithm/string/predicate.hpp>
#include </usr/include/boost/algorithm/string/trim.hpp>

struct easy_fixture
{
    easy_fixture()
    {
        easy.set(bunsan::curl::options::verbose(true));
    }

    bunsan::curl::easy easy;
};

struct url_fixture: easy_fixture
{
    url_fixture()
    {
        easy.set(bunsan::curl::options::url("http://google.com"));
    }

    void verify_data()
    {
        BOOST_CHECK(boost::algorithm::starts_with(data, "<HTML><HEAD>"));
        BOOST_CHECK(boost::algorithm::ends_with(data, "</BODY></HTML>"));
    }

    std::string data;
};

BOOST_FIXTURE_TEST_SUITE(easy_options, url_fixture)

BOOST_AUTO_TEST_CASE(call)
{
    easy.perform();
}

BOOST_AUTO_TEST_CASE(write)
{
    easy.set(bunsan::curl::options::writefunction(
        [&](char *const ptr, const std::size_t size)
        {
            data.append(ptr, size);
            return size;
        }
    ));
    easy.perform();
    BOOST_CHECK(!data.empty());
    boost::algorithm::trim(data);
    verify_data();
    BOOST_TEST_MESSAGE("HTTP Response:\n" << data);
}

BOOST_AUTO_TEST_SUITE_END() // easy_options
