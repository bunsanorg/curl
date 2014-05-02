#define BOOST_TEST_MODULE wrapper
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/options/wrapper/bitmask.hpp>
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
}

BOOST_AUTO_TEST_SUITE_END() // wrapper
