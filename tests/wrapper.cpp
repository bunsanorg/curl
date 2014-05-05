#define BOOST_TEST_MODULE wrapper
#include <boost/test/unit_test.hpp>

#include <bunsan/curl/options/wrapper/bitmask.hpp>
#include <bunsan/curl/options/wrapper/bool.hpp>
#include <bunsan/curl/options/wrapper/c_function.hpp>
#include <bunsan/curl/options/wrapper/closesocketfunction.hpp>
#include <bunsan/curl/options/wrapper/csv_list.hpp>
#include <bunsan/curl/options/wrapper/debugfunction.hpp>
#include <bunsan/curl/options/wrapper/duration.hpp>
#include <bunsan/curl/options/wrapper/enum_.hpp>
#include <bunsan/curl/options/wrapper/fnmatch_function.hpp>
#include <bunsan/curl/options/wrapper/headerfunction.hpp>
#include <bunsan/curl/options/wrapper/interleavefunction.hpp>
#include <bunsan/curl/options/wrapper/ioctlfunction.hpp>
#include <bunsan/curl/options/wrapper/literal.hpp>
#include <bunsan/curl/options/wrapper/long_.hpp>
#include <bunsan/curl/options/wrapper/opensocketfunction.hpp>
#include <bunsan/curl/options/wrapper/path.hpp>
#include <bunsan/curl/options/wrapper/progressfunction.hpp>
#include <bunsan/curl/options/wrapper/readfunction.hpp>
#include <bunsan/curl/options/wrapper/seekfunction.hpp>
#include <bunsan/curl/options/wrapper/sockoptfunction.hpp>
#include <bunsan/curl/options/wrapper/ssl_ctx_function.hpp>
#include <bunsan/curl/options/wrapper/string.hpp>
#include <bunsan/curl/options/wrapper/wrapped_option_default.hpp>
#include <bunsan/curl/options/wrapper/writefunction.hpp>

#include <boost/mpl/list.hpp>

BOOST_AUTO_TEST_SUITE(wrapper)

using namespace bunsan::curl::options::wrapper;

BOOST_AUTO_TEST_CASE(bool_)
{
    BOOST_CHECK_EQUAL(bool_01(false).data(), 0);
    BOOST_CHECK_EQUAL(bool_01(true).data(), 1);
    BOOST_CHECK_EQUAL(bool_01(false).data(), 0);
    BOOST_CHECK_EQUAL(bool_02(true).data(), 2);
}

BOOST_AUTO_TEST_CASE(bitmask_optional_)
{
    enum class type
    {
        first = 1 << 0,
        second = 1 << 1,
        third = 1 << 2,
        def = 1 << 10
    };
    typedef bitmask<type> bm;
    typedef bitmask_optional<type, type::def> bmo;

    BOOST_CHECK_EQUAL(
        bmo().data(),
        static_cast<long>(type::def)
    );
    BOOST_CHECK_EQUAL(
        bm(type::first).data(),
        static_cast<long>(type::first)
    );
    BOOST_CHECK_EQUAL(
        bmo(type::first).data(),
        static_cast<long>(type::first)
    );
    BOOST_CHECK_EQUAL(
        bm(type::first, type::third).data(),
        static_cast<long>(type::first) | static_cast<long>(type::third)
    );
    BOOST_CHECK_EQUAL(
        bmo(type::first, type::third).data(),
        static_cast<long>(type::first) | static_cast<long>(type::third)
    );

    typedef bitmask<long long> llbm;
    typedef bitmask_optional<long long, 0> llbmo;

    BOOST_CHECK_EQUAL(llbmo().data(), 0);
    BOOST_CHECK_EQUAL(llbm(1).data(), 1);
    BOOST_CHECK_EQUAL(llbmo(1).data(), 1);
    BOOST_CHECK_EQUAL(llbm(1, 2, 4).data(), 1 | 2 | 4);
    BOOST_CHECK_EQUAL(llbmo(1, 2, 4).data(), 1 | 2 | 4);
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

BOOST_AUTO_TEST_CASE(closesocketfunction_)
{
    bool cs1_ = false;
    const closesocketfunction cs1(
        [&](curl_socket_t item)
        {
            BOOST_CHECK_EQUAL(item, 15);
            cs1_ = true;
            return 10;
        });
    BOOST_CHECK_EQUAL(cs1.callback()(cs1.data(), curl_socket_t(15)), 10);
    BOOST_CHECK(cs1_);
}

BOOST_AUTO_TEST_CASE(csv_list_)
{
    BOOST_CHECK_EQUAL(csv_list<>({"hello", "world"}).data(), "hello,world");
    BOOST_CHECK_EQUAL(csv_list<>({"1", "2", "3"}).data(), "1,2,3");
    BOOST_CHECK_EQUAL(csv_list<':'>({"1", "2", "3"}).data(), "1:2:3");
    {
        typedef csv_list<';', ' '> csv_list_;
        BOOST_CHECK_EQUAL(csv_list_({"1", "2", "3"}).data(), "1; 2; 3");
    }
    {
        typedef csv_list<' ', '=', ' '> csv_list_;
        BOOST_CHECK_EQUAL(csv_list_({"1", "2", "3"}).data(), "1 = 2 = 3");
    }
}

BOOST_AUTO_TEST_CASE(debugfunction_)
{
    bunsan::curl::easy easy;
    bool d1_ = false;
    const debugfunction d1(
        [&](bunsan::curl::easy &handle,
            bunsan::curl::info info_type,
            char *ptr,
            size_t size)
        {
            BOOST_CHECK_EQUAL(&handle, &easy);
            BOOST_CHECK_EQUAL(info_type, bunsan::curl::info::data_out);
            BOOST_CHECK_EQUAL(std::string(ptr), "ptr");
            BOOST_CHECK_EQUAL(size, 15);
            d1_ = true;
        });
    char ptr[] = "ptr";
    d1.callback()(easy.handle(), CURLINFO_DATA_OUT, ptr, 15, d1.data());
    BOOST_CHECK(d1_);
}

BOOST_AUTO_TEST_CASE(duration_)
{
    BOOST_CHECK_EQUAL(seconds(std::chrono::minutes(1)).data(), 60);
    BOOST_CHECK_EQUAL(milliseconds(std::chrono::seconds(1)).data(), 1000);
}

BOOST_AUTO_TEST_CASE(enum__)
{
    enum class type
    {
        first,
        second,
        third,
        def
    };
    typedef enum_<type, type::def> enum_type;
    BOOST_CHECK_EQUAL(enum_type(type::first).data(), 0);
    BOOST_CHECK_EQUAL(enum_type(type::second).data(), 1);
    BOOST_CHECK_EQUAL(enum_type(type::third).data(), 2);
    BOOST_CHECK_EQUAL(enum_type().data(), 3);
}

BOOST_AUTO_TEST_CASE(fnmatch_function_)
{
    bool fm1_ = false;
    const fnmatch_function fm1(
        [&](const char *pattern, const char *string)
        {
            BOOST_CHECK_EQUAL(std::string(pattern), "hello");
            BOOST_CHECK_EQUAL(std::string(string), "world");
            fm1_ = true;
            return bunsan::curl::fnmatch::nomatch;
        });
    BOOST_CHECK_EQUAL(
        fm1.callback()(fm1.data(), "hello", "world"),
        CURL_FNMATCHFUNC_NOMATCH
    );
    BOOST_CHECK(fm1_);
}

BOOST_AUTO_TEST_CASE(headerfunction_)
{
    bool h1_ = false;
    const headerfunction h1(
        [&](char *ptr, size_t size)
        {
            BOOST_CHECK_EQUAL(std::string(ptr), "ptr");
            BOOST_CHECK_EQUAL(size, 15);
            h1_ = true;
            return 10;
        });
    char ptr[] = "ptr";
    BOOST_CHECK_EQUAL(h1.callback()(ptr, 3, 5, h1.data()), 10);
    BOOST_CHECK(h1_);
}

BOOST_AUTO_TEST_CASE(interleavefunction_)
{
    bool il1_ = false;
    const interleavefunction il1(
        [&](char *ptr, size_t size)
        {
            BOOST_CHECK_EQUAL(std::string(ptr), "ptr");
            BOOST_CHECK_EQUAL(size, 15);
            il1_ = true;
            return 10;
        });
    char ptr[] = "ptr";
    BOOST_CHECK_EQUAL(il1.callback()(ptr, 3, 5, il1.data()), 10);
    BOOST_CHECK(il1_);
}

BOOST_AUTO_TEST_CASE(ioctlfunction_)
{
    bunsan::curl::easy easy;
    bool io1_ = false;
    const ioctlfunction io1(
        [&](bunsan::curl::easy &handle, int cmd)
        {
            BOOST_CHECK_EQUAL(&handle, &easy);
            BOOST_CHECK_EQUAL(cmd, 15);
            io1_ = true;
            return bunsan::curl::ioerr::unknowncmd;
        });
    BOOST_CHECK_EQUAL(
        io1.callback()(easy.handle(), 15, io1.data()),
        CURLIOE_UNKNOWNCMD
    );
    BOOST_CHECK(io1_);
}

BOOST_AUTO_TEST_CASE(literal_)
{
    {
        enum class type
        {
            first = 0,
            second = 1,
            third = 2
        };
        typedef enum_literal<type, type::third> lit;
        BOOST_CHECK_EQUAL(lit().data(), 2);
    }
    {
        typedef long_literal<10> lit;
        BOOST_CHECK_EQUAL(lit().data(), 10);
    }
    {
        typedef string_literal<> lit;
        BOOST_CHECK_EQUAL(std::string(lit().data()), "");
    }
    {
        typedef string_literal<'h', 'e', 'l', 'l', 'o'> lit;
        BOOST_CHECK_EQUAL(std::string(lit().data()), "hello");
    }
}

BOOST_AUTO_TEST_CASE(long__)
{
    BOOST_CHECK_EQUAL(long_(10).data(), 10);
}

BOOST_AUTO_TEST_CASE(opensocketfunction_)
{
    bool os1_ = false;
    struct curl_sockaddr addr;
    const opensocketfunction os1(
        [&](bunsan::curl::socktype purpose, struct curl_sockaddr *address)
        {
            BOOST_CHECK_EQUAL(purpose, bunsan::curl::socktype::accept);
            BOOST_CHECK_EQUAL(address, &addr);
            os1_ = true;
            return curl_socket_t(10);
        });
    BOOST_CHECK_EQUAL(os1.callback()(os1.data(), CURLSOCKTYPE_ACCEPT, &addr), 10);
    BOOST_CHECK(os1_);
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

typedef boost::mpl::list<
    double,
    curl_off_t
> progressfunction_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(progressfunction_, T, progressfunction_types)
{
    typedef basic_progressfunction<T> test_progressfunction;
    bool pr1_ = false;
    T dltotal_ = 100;
    T dlnow_ = 50;
    T ultotal_ = 1000;
    T ulnow_ = 200;
    const test_progressfunction pr1(
        [&](T dltotal, T dlnow, T ultotal, T ulnow)
        {
            BOOST_CHECK_EQUAL(dltotal, dltotal_);
            BOOST_CHECK_EQUAL(dlnow, dlnow_);
            BOOST_CHECK_EQUAL(ultotal, ultotal_);
            BOOST_CHECK_EQUAL(ulnow, ulnow_);
            pr1_ = true;
            return 10;
        });
    BOOST_CHECK_EQUAL(pr1.callback()(pr1.data(), dltotal_, dlnow_, ultotal_, ulnow_), 10);
    BOOST_CHECK(pr1_);
}

BOOST_AUTO_TEST_CASE(readfunction_)
{
    bool r1_ = false;
    const readfunction r1(
        [&](char *ptr, size_t size)
        {
            BOOST_CHECK_EQUAL(std::string(ptr), "ptr");
            BOOST_CHECK_EQUAL(size, 12);
            r1_ = true;
            return 10;
        });
    char ptr[] = "ptr";
    BOOST_CHECK_EQUAL(r1.callback()(ptr, 3, 4, r1.data()), 10);
    BOOST_CHECK(r1_);
}

BOOST_AUTO_TEST_CASE(seekfunction_)
{
    bool s1_ = false;
    const seekfunction s1(
        [&](curl_off_t offset, int origin)
        {
            BOOST_CHECK_EQUAL(offset, 25);
            BOOST_CHECK_EQUAL(origin, SEEK_SET);
            s1_ = true;
            return bunsan::curl::seekfunc::cantseek;
        });
    BOOST_CHECK_EQUAL(s1.callback()(s1.data(), 25, SEEK_SET), CURL_SEEKFUNC_CANTSEEK);
    BOOST_CHECK(s1_);
}

BOOST_AUTO_TEST_CASE(sockoptfunction_)
{
    bool so1_ = false;
    const sockoptfunction so1(
        [&](curl_socket_t curlfd, bunsan::curl::socktype purpose)
        {
            BOOST_CHECK_EQUAL(curlfd, 12);
            BOOST_CHECK_EQUAL(purpose, bunsan::curl::socktype::accept);
            so1_ = true;
            return 10;
        });
    BOOST_CHECK_EQUAL(so1.callback()(so1.data(), curl_socket_t(12), CURLSOCKTYPE_ACCEPT), 10);
    BOOST_CHECK(so1_);
}

BOOST_AUTO_TEST_CASE(ssl_ctx_function_)
{
    bunsan::curl::easy easy;
    bool sc1_ = false;
    const ssl_ctx_function sc1(
        [&](bunsan::curl::easy &handle, void *sslctx)
        {
            BOOST_CHECK_EQUAL(&handle, &easy);
            BOOST_CHECK_EQUAL(
                std::string(static_cast<char *>(sslctx)),
                "ptr"
            );
            sc1_ = true;
            return CURLE_AGAIN;
        });
    char ptr[] = "ptr";
    BOOST_CHECK_EQUAL(sc1.callback()(easy.handle(), ptr, sc1.data()), CURLE_AGAIN);
    BOOST_CHECK(sc1_);
}

BOOST_AUTO_TEST_CASE(string_)
{
    BOOST_CHECK(string(boost::none).data() == nullptr);
    BOOST_CHECK_EQUAL(string("hello").data(), std::string("hello"));
    BOOST_CHECK_EQUAL(string("1 2 3").data(), std::string("1 2 3"));
    BOOST_CHECK_EQUAL(string("12345", 4).data(), std::string("1234"));
}

BOOST_AUTO_TEST_CASE(wrapped_option_default_)
{
    typedef wrapped_option_default<seconds, 15> def;
    BOOST_CHECK_EQUAL(def(std::chrono::seconds(10)).data(), 10);
    BOOST_CHECK_EQUAL(def(std::chrono::minutes(1)).data(), 60);
    BOOST_CHECK_EQUAL(def().data(), 15);
}

BOOST_AUTO_TEST_CASE(writefunction_)
{
    bool w1_ = false;
    const writefunction w1(
        [&](char *ptr, size_t size)
        {
            BOOST_CHECK_EQUAL(std::string(ptr), "ptr");
            BOOST_CHECK_EQUAL(size, 6);
            w1_ = true;
            return 10;
        });
    char ptr[] = "ptr";
    BOOST_CHECK_EQUAL(w1.callback()(ptr, 2, 3, w1.data()), 10);
    BOOST_CHECK(w1_);
}

BOOST_AUTO_TEST_SUITE_END() // wrapper
