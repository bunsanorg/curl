#define BOOST_TEST_MODULE wrapper
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/options/wrapper/bitmask.hpp>
#include <bunsan/curl/options/wrapper/bool.hpp>
#include <bunsan/curl/options/wrapper/c_function.hpp>
#include <bunsan/curl/options/wrapper/csv_list.hpp>
#include <bunsan/curl/options/wrapper/duration.hpp>
#include <bunsan/curl/options/wrapper/path.hpp>
#include <bunsan/curl/options/wrapper/string.hpp>

BOOST_AUTO_TEST_SUITE(wrapper)

using namespace bunsan::curl::options::wrapper;

BOOST_AUTO_TEST_CASE(bool_)
{
    BOOST_CHECK_EQUAL(bool_01(false).data(), 0);
    BOOST_CHECK_EQUAL(bool_01(true).data(), 1);
    BOOST_CHECK_EQUAL(bool_01(false).data(), 0);
    BOOST_CHECK_EQUAL(bool_02(true).data(), 2);
}

BOOST_AUTO_TEST_CASE(bitmask_)
{
    enum class type
    {
        first = 1 << 0,
        second = 1 << 1,
        third = 1 << 2,
        def = 1 << 10
    };
    typedef bitmask<type, type::def> bm;

    BOOST_CHECK_EQUAL(
        bm().data(),
        static_cast<long>(type::def)
    );
    BOOST_CHECK_EQUAL(
        bm(type::first).data(),
        static_cast<long>(type::first)
    );
    BOOST_CHECK_EQUAL(
        bm(type::first, type::third).data(),
        static_cast<long>(type::first) | static_cast<long>(type::third)
    );

    typedef bitmask<long long, 0> llbm;

    BOOST_CHECK_EQUAL(llbm().data(), 0);
    BOOST_CHECK_EQUAL(llbm(1).data(), 1);
    BOOST_CHECK_EQUAL(llbm(1, 2, 4).data(), 1 | 2 | 4);
}

BOOST_AUTO_TEST_CASE(c_function_)
{
    struct ns
    {
        static int x() { return 10; }
    };

    typedef c_function<int ()> function;

    BOOST_CHECK_EQUAL(function(&ns::x).data(), &ns::x);
    BOOST_CHECK_EQUAL(function(&ns::x).data()(), 10);
}

BOOST_AUTO_TEST_CASE(csv_list_)
{
    BOOST_CHECK_EQUAL(csv_list<>({"hello", "world"}).data(), "hello,world");
    BOOST_CHECK_EQUAL(csv_list<>({"1", "2", "3"}).data(), "1,2,3");
    BOOST_CHECK_EQUAL(csv_list<':'>({"1", "2", "3"}).data(), "1:2:3");
}

BOOST_AUTO_TEST_CASE(duration_)
{
    BOOST_CHECK_EQUAL(seconds(std::chrono::minutes(1)).data(), 60);
    BOOST_CHECK_EQUAL(milliseconds(std::chrono::seconds(1)).data(), 1000);
}

BOOST_AUTO_TEST_CASE(path_)
{
    typedef boost::filesystem::path fpath;

    BOOST_CHECK(path(boost::none).data() == nullptr);
    BOOST_CHECK_EQUAL(
        fpath(path(fpath("hello") / "world").data()),
        fpath("hello/world")
    );
    BOOST_CHECK_EQUAL(
        fpath(path("hello").data()),
        fpath("hello")
    );
}

BOOST_AUTO_TEST_CASE(string_)
{
    BOOST_CHECK(string(boost::none).data() == nullptr);
    BOOST_CHECK_EQUAL(string("hello").data(), std::string("hello"));
    BOOST_CHECK_EQUAL(string("1 2 3").data(), std::string("1 2 3"));
    BOOST_CHECK_EQUAL(string("12345", 4).data(), std::string("1234"));
}

BOOST_AUTO_TEST_SUITE_END() // wrapper
