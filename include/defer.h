#ifndef DEFER_H
#define DEFER_H

#include <type_traits>
#include <utility>

namespace detail {

template <typename... Ts>
class defer_impl : Ts... {
    public:
        defer_impl() = delete;

        template <typename... Args>
        defer_impl(Args&&... args) noexcept((std::is_nothrow_move_constructible_v<Args> && ...));

        defer_impl(defer_impl const&) = delete;

        ~defer_impl();

        defer_impl& operator=(defer_impl const&)  = delete;
    private:
        template <typename T0, typename... T1toN>
        constexpr void invoke_each() noexcept((noexcept(std::declval<Ts>()()) && ...));
};

template <typename... Ts>
template <typename... Args>
defer_impl<Ts...>::defer_impl(Args&&... args) noexcept((std::is_nothrow_move_constructible_v<Args> && ...))
    : Ts(std::forward<Args>(args))... { }

template <typename... Ts>
defer_impl<Ts...>::~defer_impl() {
    invoke_each<Ts...>();
}

template <typename... Ts>
template <typename T0, typename... T1toN>
constexpr void defer_impl<Ts...>::invoke_each() noexcept((noexcept(std::declval<Ts>()()) && ...)) {
    if constexpr(sizeof...(T1toN)) {
        static_cast<T0>(*this)();
        invoke_each<T1toN...>();
    }
    else {
        static_cast<T0>(*this)();
    }
}

template <typename... Ts>
defer_impl(Ts...) -> defer_impl<Ts...>;

} /* namespace detail */

#define DEFER_CAT(a,b) a##b
#define DEFER_CAT_EXPAND(a,b) DEFER_CAT(a,b)

#define defer(...) auto const DEFER_CAT_EXPAND(internal_defer_var_,__LINE__) = detail::defer_impl(__VA_ARGS__)

#endif /* DEFER_H */
