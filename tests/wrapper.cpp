#define BOOST_TEST_MODULE wrapper
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/options/wrapper/bool.hpp>

BOOST_AUTO_TEST_SUITE(wrapper)

using namespace bunsan::curl::options::wrapper;

BOOST_AUTO_TEST_CASE(bool_)
{
    BOOST_CHECK_EQUAL(bool_01(false).data(), 0);
    BOOST_CHECK_EQUAL(bool_01(true).data(), 1);
    BOOST_CHECK_EQUAL(bool_01(false).data(), 0);
    BOOST_CHECK_EQUAL(bool_02(true).data(), 2);
}

BOOST_AUTO_TEST_SUITE_END() // wrapper
