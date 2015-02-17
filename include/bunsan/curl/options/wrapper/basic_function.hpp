#pragma once

#include <utility>

namespace bunsan{namespace curl{namespace options{namespace wrapper
{
    template <typename Traits>
    class basic_function
    {
    public:
        using function_type = typename Traits::function_type;
        using result_type = typename function_type::result_type;

        template <typename Arg, typename ... Args>
        explicit basic_function(Arg &&arg, Args &&...args):
            m_function(
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
        result_type call(Args &&...args) const
        {
            return m_function(std::forward<Args>(args)...);
        }

    private:
        function_type m_function;
    };
}}}}
