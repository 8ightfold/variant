#ifndef VARIANT_VARIANT_HPP
#define VARIANT_VARIANT_HPP

#include <exception>
#include <variant/detail/node_interleave.hpp>

struct bad_variant_access : std::exception {
    [[nodiscard]]
    const char* what() const noexcept override {
        return "bad variant access: extracted as incorrect type.";
    }
};

template <typename...TT>
struct variant : detail::variant_nodes<TT...> {
    // constructors/destructor/assignment --------------------------------------;
    constexpr variant() noexcept : _data{ nullptr, typeid(void).hash_code() } {}

    constexpr variant(const variant& v) : _data( this->copy_type(v._data) ) {}

    constexpr variant(variant&& v) noexcept : _data( v._data ) { v._data = {}; }

    template <typename T>
    constexpr variant(T&& t) requires (not std::same_as<T, variant&>)
            : _data( this->construct_type(std::forward<T>(t)) ) {}

    constexpr ~variant() { if(_data.data) this->destruct_type(_data); }

    constexpr variant& operator=(const variant& v) {
        if(_data.data) this->destruct_type(_data);
        _data = this->copy_type(v._data);
    }

    constexpr variant& operator=(variant&& v) noexcept {
        _data = v._data;
        v._data = {};
    }

    // observers ---------------------------------------------------------------;
    constexpr std::size_t index() const noexcept { return _data.index; }

    // modifiers ---------------------------------------------------------------;
    template <typename T, typename...Args>
    constexpr T& emplace(Args&&...args) {
        if(_data.data) this->destruct_type(_data);
        _data = this->template construct_type_with<T>(std::forward<Args>(args)...);
        return *static_cast<T*>(_data.data);
    }

    constexpr void swap(variant& v) noexcept {
        auto this_data = _data;
        _data = v._data;
        v._data = this_data;
    }

    // accessors ---------------------------------------------------------------;
    template <typename F>
    constexpr auto visit(F&& callable) {
        return this->invoke_type(_data, std::forward<F>(callable));
    }

    template <typename T>
    constexpr bool contains() noexcept {
        return typeid(T).hash_code() == _data.hash_code;
    }

    template <std::size_t N>
    constexpr auto operator[](const detail::index_constant<N> id) {
        auto* data = this->get_type(id, _data);
        if (typeid(*data).hash_code() == _data.hash_code) return *data;
        else throw bad_variant_access{};
    }

    template <typename T>
    constexpr auto operator[](const detail::type_constant<T> tp) {
        auto* data = this->get_type(tp, _data);
        if (typeid(*data).hash_code() == _data.hash_code) return *data;
        else throw bad_variant_access{};
    }

private:
    detail::variant_data _data;
};

#endif //VARIANT_VARIANT_HPP
