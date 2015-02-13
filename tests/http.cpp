#define BOOST_TEST_MODULE http
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/http/error.hpp>
#include <bunsan/curl/http/header.hpp>
#include <bunsan/curl/http/status.hpp>
#include <bunsan/curl/http_version.hpp>

namespace curl = bunsan::curl;
namespace http = curl::http;

BOOST_AUTO_TEST_SUITE(http_)

BOOST_AUTO_TEST_CASE(make_version)
{
    BOOST_CHECK_EQUAL(curl::make_http_version(1, 0), curl::http_version::http_1_0);
    BOOST_CHECK_EQUAL(curl::make_http_version(1, 1), curl::http_version::http_1_1);
    BOOST_CHECK_EQUAL(curl::make_http_version(2, 0), curl::http_version::http_2_0);
    BOOST_CHECK_THROW(curl::make_http_version(1, 3), curl::http_version_unsupported_error);
}

BOOST_AUTO_TEST_CASE(status_parse)
{
    auto r = curl::http::status::parse("HTTP/1.1 200 OK");
    BOOST_CHECK_EQUAL(r.version, curl::http_version::http_1_1);
    BOOST_CHECK_EQUAL(r.code, 200);
    BOOST_CHECK_EQUAL(r.reason, "OK");

    r = curl::http::status::parse("HTTP/1.0 300 hello world");
    BOOST_CHECK_EQUAL(r.version, curl::http_version::http_1_0);
    BOOST_CHECK_EQUAL(r.code, 300);
    BOOST_CHECK_EQUAL(r.reason, "hello world");

    r = curl::http::status::parse("HTTP/2.0 100");
    BOOST_CHECK_EQUAL(r.version, curl::http_version::http_2_0);
    BOOST_CHECK_EQUAL(r.code, 100);
    BOOST_CHECK(r.reason.empty());

    r = curl::http::status::parse("HTTP/1.1");
    BOOST_CHECK_EQUAL(r.version, curl::http_version::http_1_1);
    BOOST_CHECK(!r.code);
    BOOST_CHECK(r.reason.empty());

    r = curl::http::status::parse("HTTP/1.1 ");
    BOOST_CHECK_EQUAL(r.version, curl::http_version::http_1_1);
    BOOST_CHECK(!r.code);
    BOOST_CHECK(r.reason.empty());

    BOOST_CHECK_THROW(curl::http::status::parse("HTTP/1.3 200 OK"),
                      curl::http::status_parse_error);

    BOOST_CHECK_THROW(curl::http::status::parse("HTTP/1"),
                      curl::http::status_parse_error);

    BOOST_CHECK_THROW(curl::http::status::parse("HTTP/-1"),
                      curl::http::status_parse_error);

    BOOST_CHECK_THROW(curl::http::status::parse("HTTP"),
                      curl::http::status_parse_error);

    BOOST_CHECK_THROW(curl::http::status::parse("HTTPS/1.0 200 OK"),
                      curl::http::status_parse_error);
}

BOOST_AUTO_TEST_SUITE(header)

BOOST_AUTO_TEST_CASE(case_sensitivity)
{
    BOOST_CHECK_EQUAL(http::header::parse("Content-Type: text/plain"),
                      http::header("content-type", "text/plain"));
    BOOST_CHECK_EQUAL(http::header::parse("Header: Data"),
                      http::header("header", "Data"));
}

BOOST_AUTO_TEST_CASE(spaces)
{
    BOOST_CHECK_EQUAL(http::header::parse("Header: Data   "),
                      http::header("header", "Data"));
    BOOST_CHECK_EQUAL(http::header::parse("Header:Data"),
                      http::header("header", "Data"));
    BOOST_CHECK_EQUAL(http::header::parse("Header:Data   "),
                      http::header("header", "Data"));
    BOOST_CHECK_EQUAL(http::header::parse("Header   :Data"),
                      http::header("header", "Data"));
    BOOST_CHECK_EQUAL(http::header::parse("Header   :Data   "),
                      http::header("header", "Data"));
    BOOST_CHECK_EQUAL(http::header::parse("Header   :  Data   "),
                      http::header("header", "Data"));
}

BOOST_AUTO_TEST_SUITE_END() // header

BOOST_AUTO_TEST_SUITE_END() // http_
