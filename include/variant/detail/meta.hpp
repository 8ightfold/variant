#ifndef VARIANT_META_HPP
#define VARIANT_META_HPP

#include <type_traits>
#include <utility>

namespace detail {
    template <std::size_t N>
    using seq_t = decltype(std::make_index_sequence<N>{});

    template <std::size_t N>
    using index_constant = std::integral_constant<std::size_t, N>;

    template <typename T>
    struct type_constant { using type = T; };
}

template <std::size_t N>
inline constexpr detail::index_constant<N> I {};

template <typename T>
inline constexpr detail::type_constant<T> Tp {};

#endif //VARIANT_META_HPP
