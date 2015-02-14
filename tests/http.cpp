#define BOOST_TEST_MODULE http
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/http/error.hpp>
#include <bunsan/curl/http/header.hpp>
#include <bunsan/curl/http/status.hpp>
#include <bunsan/curl/http_version.hpp>

#include <boost/lexical_cast.hpp>

namespace curl = bunsan::curl;
namespace http = curl::http;

BOOST_AUTO_TEST_SUITE(http_)

BOOST_AUTO_TEST_SUITE(http_version)

BOOST_AUTO_TEST_CASE(pair_to_enum)
{
    BOOST_CHECK_EQUAL(curl::http_version_pair(1, 0), curl::http_version::http_1_0);
    BOOST_CHECK_EQUAL(curl::http_version_pair(1, 1), curl::http_version::http_1_1);
    BOOST_CHECK_EQUAL(curl::http_version_pair(2, 0), curl::http_version::http_2_0);
    BOOST_CHECK_THROW(
        static_cast<curl::http_version>(curl::http_version_pair(1, 3)),
        curl::http_version_unsupported_error
    );
}

BOOST_AUTO_TEST_CASE(enum_to_pair)
{
    BOOST_CHECK_EQUAL(curl::http_version_pair(curl::http_version::http_none),
                      curl::http_version_pair());
    BOOST_CHECK_EQUAL(curl::http_version_pair(curl::http_version::http_1_0),
                      curl::http_version_pair(1, 0));
    BOOST_CHECK_EQUAL(curl::http_version_pair(curl::http_version::http_1_1),
                      curl::http_version_pair(1, 1));
    BOOST_CHECK_EQUAL(curl::http_version_pair(curl::http_version::http_2_0),
                      curl::http_version_pair(2, 0));
}

BOOST_AUTO_TEST_CASE(stream)
{
    BOOST_CHECK_EQUAL(
        boost::lexical_cast<std::string>(curl::http_version_pair()),
        "0.0"
    );
    BOOST_CHECK_EQUAL(
        boost::lexical_cast<std::string>(curl::http_version_pair(1, 2)),
        "1.2"
    );
    BOOST_CHECK_EQUAL(
        boost::lexical_cast<std::string>(
            curl::http_version_pair(curl::http_version::http_1_1)
        ),
        "1.1"
    );
}

BOOST_AUTO_TEST_SUITE_END() // http_version

BOOST_AUTO_TEST_SUITE(status)

BOOST_AUTO_TEST_CASE(stream)
{
    BOOST_CHECK_EQUAL(
        boost::lexical_cast<std::string>(
            curl::http::status(curl::http_version::http_1_1, 200, "OK")
        ),
        "HTTP/1.1 200 OK"
    );
    BOOST_CHECK_EQUAL(
        boost::lexical_cast<std::string>(
            curl::http::status(curl::http_version::http_1_1, 200, "")
        ),
        "HTTP/1.1 200"
    );
    BOOST_CHECK_EQUAL(
        boost::lexical_cast<std::string>(
            curl::http::status(curl::http_version::http_1_1, 0, "")
        ),
        "HTTP/1.1"
    );
}

BOOST_AUTO_TEST_CASE(parse)
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

BOOST_AUTO_TEST_SUITE_END() // status

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

BOOST_AUTO_TEST_CASE(stream)
{
    const http::header u("header", "data1");
    const http::header h("Header", "data1", "data2");
    BOOST_CHECK_EQUAL(
        boost::lexical_cast<std::string>(u),
        "header: data1"
    );
    BOOST_CHECK_EQUAL(
        boost::lexical_cast<std::string>(h),
        "header: [\"data1\", \"data2\"]"
    );
}

BOOST_AUTO_TEST_CASE(non_unique_value)
{
    const http::header u("header", "data1");
    const http::header h("Header", "data1", "data2");
    BOOST_CHECK_NE(u, h);
    BOOST_CHECK_EQUAL(h, http::header("header", "data1", "data2"));
    BOOST_CHECK_EQUAL(u.value(), "data1");
    BOOST_CHECK_THROW(h.value(), http::header_non_unique_value_error);
}

BOOST_AUTO_TEST_CASE(merge)
{
    http::header h("header", "data1");
    BOOST_CHECK_THROW(
        h.merge(http::header("other", "data2")),
        http::header_merge_incompatible_name_error
    );
    h.merge(http::header("Header", "data2"));
    const http::header h3("header", "data3");
    h.merge(h3);
    BOOST_CHECK_EQUAL(
        h,
        http::header("header", "data1", "data2", "data3")
    );
    BOOST_CHECK_EQUAL(
        http::merge_headers(
            h,
            http::header("header", "data4"),
            http::header("header", "data5")),
        http::header("header", "data1", "data2", "data3", "data4", "data5")
    );
}

BOOST_AUTO_TEST_SUITE_END() // header

BOOST_AUTO_TEST_SUITE_END() // http_
