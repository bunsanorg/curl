#include <bunsan/curl/detail/string_list.hpp>

namespace bunsan{namespace curl{namespace detail
{
    string_list::string_list(const string_list &list)
    {
        for (const char *const obj: list)
            append(obj);
    }

    string_list &string_list::operator=(const string_list &list)
    {
        string_list(list).swap(*this);
        return *this;
    }
}}}
