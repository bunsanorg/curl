#define BOOST_TEST_MODULE easy_options
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/easy.hpp>
#include <bunsan/curl/options/behavior.hpp>
#include <bunsan/curl/options/callback.hpp>
#include <bunsan/curl/options/connection.hpp>
#include <bunsan/curl/options/http.hpp>
#include <bunsan/curl/options/network.hpp>
#include <bunsan/curl/options/protocol.hpp>

#include </usr/include/boost/algorithm/string/predicate.hpp>
#include </usr/include/boost/algorithm/string/trim.hpp>

#include <cstring>

struct easy_fixture
{
    bunsan::curl::easy easy;
    const std::string url_root = "http://localhost:8090";
    std::string data;
};

BOOST_FIXTURE_TEST_SUITE(easy_options, easy_fixture)

BOOST_AUTO_TEST_CASE(write)
{
    easy.set(bunsan::curl::options::url(url_root + "/hello"));
    easy.set(bunsan::curl::options::writefunction(
        [&](char *const ptr, const std::size_t size)
        {
            data.append(ptr, size);
            return size;
        }
    ));
    easy.perform();
    BOOST_CHECK_EQUAL(data, "Hello, world!");
}

BOOST_AUTO_TEST_CASE(timeout)
{
    easy.set(bunsan::curl::options::url(url_root + "/sleep"));
    easy.set(bunsan::curl::options::timeout(std::chrono::milliseconds(100)));

    std::error_code ec;
    easy.perform(ec);
    BOOST_CHECK_EQUAL(ec, CURLE_OPERATION_TIMEDOUT);
}

BOOST_AUTO_TEST_CASE(maxfilesize)
{
    easy.set(bunsan::curl::options::url(url_root + "/big"));
    easy.set(bunsan::curl::options::maxfilesize(1024 * 1024));

    std::error_code ec;
    easy.perform(ec);
    BOOST_CHECK_EQUAL(ec, CURLE_FILESIZE_EXCEEDED);
}

BOOST_AUTO_TEST_CASE(postfields)
{
    easy.set(bunsan::curl::options::url(url_root + "/echo"));
    easy.set(bunsan::curl::options::postfields("Hello, world!"));

    easy.set(bunsan::curl::options::writefunction(
        [&](char *const ptr, const std::size_t size)
        {
            data.append(ptr, size);
            return size;
        }
    ));

    easy.perform();
    BOOST_CHECK_EQUAL(data, "Hello, world!");
}

BOOST_AUTO_TEST_SUITE_END() // easy_options
