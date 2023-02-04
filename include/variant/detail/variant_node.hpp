#ifndef VARIANT_VARIANT_NODE_HPP
#define VARIANT_VARIANT_NODE_HPP

#include <typeinfo>
#include <variant/detail/meta.hpp>

namespace detail {
    struct variant_data {
        void *data = nullptr;
        std::size_t hash_code;
        std::size_t index = -1;
    };

    template <std::size_t N, typename T>
    struct variant_node {
        static constexpr variant_data construct(T& t) {
            T* data = new T { t };
            std::size_t hash_code = typeid(T).hash_code();
            return {data, hash_code, N};
        }

        static constexpr variant_data construct(T&& t) {
            T* data = new T { std::move(t) };
            std::size_t hash_code = typeid(T).hash_code();
            return { data, hash_code, N };
        }

        template<typename...Args>
        static constexpr variant_data construct_with(const type_constant<T>, Args&&...args) {
            T* data = new T { std::forward<Args>(args)... };
            std::size_t hash_code = typeid(T).hash_code();
            return { data, hash_code, N };
        }

        static constexpr void copy(const index_constant<N>, const variant_data& d, variant_data& new_d) {
            auto* old_data = static_cast<T*>(d.data);
            new_d.data = new T{ *old_data };
            new_d.hash_code = d.hash_code;
            new_d.index = N;
        }

        static constexpr void destruct(const index_constant<N>, variant_data& d) noexcept {
            auto* data = static_cast<T*>(d.data);
            delete data;
            d = {};
        }

        static constexpr auto invoke(const index_constant<N>, variant_data& d, auto&& callable) {
            auto* cast = static_cast<T*>(d.data);
            return callable(*cast);
        }

        static constexpr T* get(const index_constant<N>, variant_data& d) noexcept {
            return static_cast<T*>(d.data);
        }

        static constexpr T* get(const type_constant<T>, variant_data& d) noexcept {
            return static_cast<T*>(d.data);
        }
    };
}

#endif //VARIANT_VARIANT_NODE_HPP
