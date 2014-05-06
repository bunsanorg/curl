#define BOOST_TEST_MODULE slist
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/detail/slist.hpp>
#include <bunsan/curl/detail/string_list.hpp>

BOOST_AUTO_TEST_SUITE(slist)

BOOST_AUTO_TEST_CASE(raw)
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

BOOST_AUTO_TEST_CASE(string_list)
{
    const char *objs[] = {
        "hello",
        "world",
        "of",
        "strings"
    };
    bunsan::curl::detail::string_list list;
    for (const char *const obj: objs)
        list.append(obj);

    {
        const char **iter = objs;
        for (const char *const obj: list)
        {
            BOOST_CHECK_EQUAL(std::string(obj), *iter);
            ++iter;
        }
    }

    {
        auto iter = list.begin();
        BOOST_REQUIRE(iter != list.end());
        BOOST_CHECK_EQUAL(std::string(*iter), "hello");

        ++iter;
        BOOST_REQUIRE(iter != list.end());
        BOOST_CHECK_EQUAL(std::string(*iter), "world");

        ++iter;
        BOOST_REQUIRE(iter != list.end());
        BOOST_CHECK_EQUAL(std::string(*iter), "of");

        ++iter;
        BOOST_REQUIRE(iter != list.end());
        BOOST_CHECK_EQUAL(std::string(*iter), "strings");

        ++iter;
        BOOST_REQUIRE(iter == list.end());
    }

    bunsan::curl::detail::string_list list2 = list;
    {
        auto iter = list2.begin();
        BOOST_REQUIRE(iter != list.end());
        BOOST_CHECK_EQUAL(std::string(*iter), "hello");

        ++iter;
        BOOST_REQUIRE(iter != list.end());
        BOOST_CHECK_EQUAL(std::string(*iter), "world");

        ++iter;
        BOOST_REQUIRE(iter != list.end());
        BOOST_CHECK_EQUAL(std::string(*iter), "of");

        ++iter;
        BOOST_REQUIRE(iter != list.end());
        BOOST_CHECK_EQUAL(std::string(*iter), "strings");

        ++iter;
        BOOST_REQUIRE(iter == list.end());
    }
}

BOOST_AUTO_TEST_SUITE_END() // slist
