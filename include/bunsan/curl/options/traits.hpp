#pragma once

namespace bunsan{namespace curl{namespace options
{
    namespace retention_policy
    {
        struct by_wrapper {};
        struct by_curl {};
    }

    template <typename Option>
    struct option_traits
    {
        using retention_policy = typename Option::retention_policy;
    };
}}}
