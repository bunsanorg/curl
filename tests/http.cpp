#define BOOST_TEST_MODULE http
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/http_version.hpp>

namespace curl = bunsan::curl;

BOOST_AUTO_TEST_SUITE(http)

BOOST_AUTO_TEST_CASE(make_version)
{
    BOOST_CHECK_EQUAL(curl::make_http_version(1, 0), curl::http_version::http_1_0);
    BOOST_CHECK_EQUAL(curl::make_http_version(1, 1), curl::http_version::http_1_1);
    BOOST_CHECK_EQUAL(curl::make_http_version(2, 0), curl::http_version::http_2_0);
    BOOST_CHECK_THROW(curl::make_http_version(1, 3), curl::http_version_unsupported_error);
}

BOOST_AUTO_TEST_SUITE_END() // http
