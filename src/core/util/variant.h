#pragma once

#include <cstddef>
#include <type_traits>
#include <variant>

namespace tst::core {

template<typename Type>
struct is_variant : std::false_type {};

template<typename... Types>
struct is_variant<std::variant<Types...>> : std::true_type {};

template<typename Type>
inline constexpr bool is_variant_v = is_variant<std::remove_cvref_t<Type>>::value;

namespace variant_detail {

    template<typename Type, typename Variant, size_t Index = 0>
    consteval size_t variant_index() {
        if constexpr (Index == std::variant_size_v<Variant>) {
            static_assert(Index != std::variant_size_v<Variant>, "Type must be an alternative of Variant");
        } else if constexpr (std::is_same_v<std::remove_cvref_t<Type>, std::variant_alternative_t<Index, Variant>>) {
            return Index;
        } else {
            return variant_index<Type, Variant, Index + 1>();
        }
    }

} // namespace variant_detail

template<typename Type, typename Variant>
consteval size_t variant_index() {
    return variant_detail::variant_index<Type, std::remove_cvref_t<Variant>>();
}

} // namespace tst::core
