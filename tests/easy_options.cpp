#define BOOST_TEST_MODULE easy_options
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/easy.hpp>
#include <bunsan/curl/options/behavior.hpp>
#include <bunsan/curl/options/network.hpp>

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
};

BOOST_FIXTURE_TEST_SUITE(easy_options, url_fixture)

BOOST_AUTO_TEST_CASE(call)
{
    easy.perform();
}

BOOST_AUTO_TEST_SUITE_END() // easy_options
