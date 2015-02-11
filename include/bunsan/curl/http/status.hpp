#pragma once

#include <bunsan/curl/http_version.hpp>

#include <string>

namespace bunsan{namespace curl{namespace http
{
    struct status
    {
        http_version version = http_version::http_none;
        unsigned code = 0;
        std::string reason;

        static status parse(const std::string &header);
    };
}}}
