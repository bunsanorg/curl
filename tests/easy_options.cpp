#define BOOST_TEST_MODULE easy_options
#include <boost/test/unit_test.hpp>

#include "easy_fixture.hpp"

#include <bunsan/curl/easy.hpp>
#include <bunsan/curl/options/behavior.hpp>
#include <bunsan/curl/options/callback.hpp>
#include <bunsan/curl/options/connection.hpp>
#include <bunsan/curl/options/http.hpp>
#include <bunsan/curl/options/network.hpp>
#include <bunsan/curl/options/protocol.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <cstring>

namespace curl = bunsan::curl;
namespace options = curl::options;

BOOST_FIXTURE_TEST_SUITE(easy_options, easy_fixture)

BOOST_AUTO_TEST_CASE(write)
{
    easy.set(options::url(url_root + "/hello"));
    easy.set(options::writefunction(data_writer));
    easy.perform();
    BOOST_CHECK_EQUAL(data, "Hello, world!");
}

BOOST_AUTO_TEST_CASE(timeout)
{
    easy.set(options::url(url_root + "/sleep"));
    easy.set(options::timeout(std::chrono::milliseconds(100)));

    std::error_code ec;
    easy.perform(ec);
    BOOST_CHECK_EQUAL(ec, CURLE_OPERATION_TIMEDOUT);
}

BOOST_AUTO_TEST_CASE(maxfilesize)
{
    easy.set(options::url(url_root + "/big"));
    easy.set(options::maxfilesize(1024 * 1024));

    std::error_code ec;
    easy.perform(ec);
    BOOST_CHECK_EQUAL(ec, CURLE_FILESIZE_EXCEEDED);
}

BOOST_AUTO_TEST_CASE(postfields)
{
    easy.set(options::url(url_root + "/echo"));
    easy.set(options::postfields("Hello, world!"));
    easy.set(options::writefunction(data_writer));
    easy.perform();
    BOOST_CHECK_EQUAL(data, "Hello, world!");
}

BOOST_AUTO_TEST_CASE(httpheader)
{
    easy.set(options::url(url_root + "/header"));
    easy.set(options::httpheader({"X-cURL-Test: header data"}));
    easy.set(options::writefunction(data_writer));
    easy.perform();
    BOOST_CHECK_EQUAL(data, "header data");
}

BOOST_AUTO_TEST_CASE(readfunction_with_size)
{
    const char mydata[] = "Hello, world!";
    const std::size_t mydata_size = std::strlen(mydata);
    const std::string mydata_size_str =
        boost::lexical_cast<std::string>(mydata_size);
    std::size_t mydata_pos = 0;
    easy.set(options::url(url_root + "/echo"));
    easy.set(options::post(true));
    easy.set(options::httpheader({
        "Content-Length: " + mydata_size_str
    }));
    easy.set(options::readfunction(
        [&](char *data, std::size_t size)
        {
            const std::size_t sendsize =
                std::min(size, mydata_size - mydata_pos);
            std::copy(
                mydata + mydata_pos,
                mydata + mydata_pos + sendsize,
                data
            );
            mydata_pos += sendsize;
            return sendsize;
        }));
    easy.set(options::writefunction(data_writer));
    easy.perform();
    BOOST_CHECK_EQUAL(data, mydata);
}

BOOST_AUTO_TEST_CASE(readfunction_chunked)
{
    const char mydata[] = "Hello, world!";
    const std::size_t mydata_size = std::strlen(mydata);
    std::size_t mydata_pos = 0;
    easy.set(options::url(url_root + "/echo"));
    easy.set(options::post(true));
    easy.set(options::httpheader({"Transfer-Encoding: chunked"}));
    easy.set(options::readfunction(
        [&](char *data, std::size_t size)
        {
            const std::size_t sendsize =
                std::min(size, mydata_size - mydata_pos);
            std::copy(
                mydata + mydata_pos,
                mydata + mydata_pos + sendsize,
                data
            );
            mydata_pos += sendsize;
            return sendsize;
        }));
    easy.set(options::writefunction(data_writer));
    easy.perform();
    BOOST_CHECK_EQUAL(data, mydata);
}

BOOST_AUTO_TEST_SUITE_END() // easy_options
