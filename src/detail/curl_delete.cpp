#include <bunsan/curl/detail/curl_delete.hpp>

#include <curl/curl.h>

void bunsan::curl::detail::curl_delete::operator()(void *const ptr) const
{
    ::curl_free(ptr);
}
