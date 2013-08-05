#define BOOST_TEST_MODULE easy
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/easy.hpp>

BOOST_AUTO_TEST_SUITE(easy)

BOOST_AUTO_TEST_CASE(escaping)
{
    static const std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"123", "123"},
        {"string with spaces", "string%20with%20spaces"},
        {"multiline\nstring", "multiline%0Astring"},
        {std::string("string\0with\0zeroes", 18), "string%00with%00zeroes"},
    };

    bunsan::curl::easy curl;

    for (const std::pair<std::string, std::string> &test: tests)
    {
        const std::string &unescaped = test.first;
        const std::string &escaped = test.second;
        const std::string escaped_ = curl.escape(unescaped);
        const std::string unescaped_ = curl.unescape(escaped_);
        BOOST_CHECK_EQUAL(escaped_, escaped);
        BOOST_CHECK_EQUAL(unescaped_, unescaped);
    }
}

BOOST_AUTO_TEST_SUITE_END() // easy
