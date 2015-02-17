#pragma once

#include <type_traits>

namespace bunsan{namespace curl{namespace options
{
    namespace detail
    {
        struct is_function_checker
        {
            template <typename Option>
            static std::true_type check(
                Option *,
                typename Option::function_type *f
            );

            template <typename Option>
            static std::false_type check(Option *, ...);
        };

        template <typename Option>
        struct is_function:
            decltype(
                detail::is_function_checker::check(
                    static_cast<Option *>(nullptr),
                    nullptr
                )
            ) {};
    }

    namespace retention_policy
    {
        struct by_wrapper {};
        struct by_curl {};
    }

    template <typename Option>
    struct option_traits
    {
        using retention_policy = typename Option::retention_policy;
        using is_function = detail::is_function<Option>;
    };
}}}
