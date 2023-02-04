#ifndef VARIANT_TEST_NODE_INTERLEAVE_HPP
#define VARIANT_TEST_NODE_INTERLEAVE_HPP

#include <variant/detail/variant_node.hpp>

namespace detail {
    template <typename, typename...> struct variant_node_interleave;

    template <std::size_t...NN, typename...TT>
    struct variant_node_interleave<std::index_sequence<NN...>, TT...> : variant_node<NN, TT>...
    {
        using variant_node<NN, TT>::construct...;
        using variant_node<NN, TT>::construct_with...;
        using variant_node<NN, TT>::copy...;
        using variant_node<NN, TT>::destruct...;
        using variant_node<NN, TT>::invoke...;
        using variant_node<NN, TT>::get...;

        template <typename T>
        static constexpr variant_data construct_type(T&& t) {
            return construct(std::forward<T>(t));
        }

        template <typename T, typename...Args>
        static constexpr variant_data construct_type_with(Args&&...args) {
            return construct_with(Tp<T>, std::forward<Args>(args)...);
        }

        static constexpr variant_data copy_type(const variant_data& d) {
            variant_data new_d {};
            (((d.index == NN) && (copy(I<NN>, d, new_d), true)), ...);
            return new_d;
        }

        static constexpr void destruct_type(variant_data& d) noexcept {
            (((d.index == NN) && (destruct(I<NN>, d), true)), ...);
        }

        template <typename F>
        static constexpr auto invoke_type(variant_data& d, F&& callable) {
            (((d.index == NN) && (invoke(I<NN>, d, std::forward<F>(callable)), true)), ...);
        }

        template <std::size_t N>
        static constexpr auto get_type(const index_constant<N> id, variant_data& data) noexcept {
            return get(id, data);
        }

        template <typename T>
        static constexpr auto get_type(const type_constant<T> tp, variant_data& data) noexcept {
            return get(tp, data);
        }
    };

    template <typename...TT>
    struct variant_nodes : variant_node_interleave<seq_t<sizeof...(TT)>, TT...> {};
}

#endif //VARIANT_TEST_NODE_INTERLEAVE_HPP
