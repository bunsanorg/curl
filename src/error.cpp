#include "bunsan/curl/error.hpp"
#include "bunsan/curl/error_category.hpp"

namespace bunsan{namespace curl
{
    easy_error::easy_error(const CURLcode code):
        easy_error(std::error_code(code, easy_category())) {}

    easy_error::easy_error(const CURLcode code, const std::string &what):
        easy_error(std::error_code(code, easy_category()), what) {}

    easy_error::easy_error(const std::error_code &ec):
        categorized_error(ec) {}

    easy_error::easy_error(const std::error_code &ec, const std::string &what):
        categorized_error(ec, what) {}

    multi_error::multi_error(const CURLcode code):
        multi_error(std::error_code(code, multi_category())) {}

    multi_error::multi_error(const CURLcode code, const std::string &what):
        multi_error(std::error_code(code, multi_category()), what) {}

    multi_error::multi_error(const std::error_code &ec):
        categorized_error(ec) {}

    multi_error::multi_error(const std::error_code &ec, const std::string &what):
        categorized_error(ec, what) {}
}}
