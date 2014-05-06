#define BOOST_TEST_MODULE slist
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/detail/slist.hpp>

BOOST_AUTO_TEST_SUITE(slist)

BOOST_AUTO_TEST_CASE(append_)
{
    bunsan::curl::detail::slist_ptr list;
    bunsan::curl::detail::slist::append(list, "hello");
    bunsan::curl::detail::slist::append(list, "world");

    {
        auto iter = list.get();
        BOOST_REQUIRE(iter);
        BOOST_CHECK_EQUAL(iter->data, "hello");

        iter = iter->next;
        BOOST_REQUIRE(iter);
        BOOST_CHECK_EQUAL(iter->data, "world");

        iter = iter->next;
        BOOST_REQUIRE(!iter);
    }
}

BOOST_AUTO_TEST_SUITE_END() // slist
