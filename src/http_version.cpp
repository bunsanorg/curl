#include <bunsan/curl/http_version.hpp>

namespace bunsan{namespace curl
{
    http_version make_http_version(const unsigned version_major,
                                   const unsigned version_minor)
    {
        switch (version_major)
        {
        case 1:
            switch (version_minor)
            {
            case 0:
                return http_version::http_1_0;
            case 1:
                return http_version::http_1_1;
            }
            break;
        case 2:
            switch (version_minor)
            {
            case 0:
                return http_version::http_2_0;
            }
            break;
        }
        BOOST_THROW_EXCEPTION(
            http_version_unsupported_error() <<
            http_version_unsupported_error::version_major(version_major) <<
            http_version_unsupported_error::version_minor(version_minor));
    }
}}
