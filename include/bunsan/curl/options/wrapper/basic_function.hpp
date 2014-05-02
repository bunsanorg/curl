#pragma once

#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <typename Traits>
    class basic_function
    {
    public:
        template <typename Arg, typename ... Args>
        explicit basic_function(Arg &&arg, Args &&...args):
            m_callback(
                std::forward<Arg>(arg),
                std::forward<Args>(args)...
            ) {}

        decltype(&Traits::static_call) callback() const
        {
            return &Traits::static_call;
        }

        void *data() const
        {
            // cURL's interface requires (void *)
            return const_cast<basic_function<Traits> *>(this);
        }

        template <typename ... Args>
        typename Traits::function_type::
        result_type call(Args &&...args) const
        {
            return m_callback(std::forward<Args>(args)...);
        }

    private:
        typename Traits::function_type m_callback;
    };
}}}}
