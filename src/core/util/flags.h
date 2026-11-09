#pragma once

#include <compare>
#include <type_traits>

namespace tst::core {

template<typename FlagBitsType>
struct flag_traits {
    using mask_type = std::underlying_type_t<FlagBitsType>;

    static constexpr mask_type all_flags = 0;
};

template<typename BitType>
class flags {
public:
    static_assert(std::is_enum_v<BitType>, "BitType must be an enum type");
    using MaskType = typename std::underlying_type<BitType>::type;

    // constructors
    constexpr flags() noexcept : m_mask(0) {
    }

    constexpr flags(BitType bit) noexcept : m_mask(static_cast<MaskType>(bit)) {
    }

    constexpr flags(flags const&) noexcept = default;

    constexpr explicit flags(MaskType flags) noexcept : m_mask(flags) {
    }

public:
    auto operator<=>(flags<BitType> const&) const = default;

    constexpr bool operator!() const noexcept {
        return !m_mask;
    }

    [[nodiscard]] constexpr flags<BitType> operator&(flags<BitType> rhs) const noexcept {
        return flags<BitType>(m_mask & rhs.m_mask);
    }

    [[nodiscard]] constexpr flags<BitType> operator|(flags<BitType> rhs) const noexcept {
        return flags<BitType>(m_mask | rhs.m_mask);
    }

    [[nodiscard]] constexpr flags<BitType> operator^(flags<BitType> rhs) const noexcept {
        return flags<BitType>(m_mask ^ rhs.m_mask);
    }

    [[nodiscard]] constexpr flags<BitType> operator~() const noexcept {
        return flags<BitType>(static_cast<MaskType>(~m_mask & flag_traits<BitType>::all_flags));
    }

    [[nodiscard]] constexpr flags& operator=(flags const&) noexcept = default;

    constexpr flags<BitType>& operator|=(flags<BitType> rhs) noexcept {
        m_mask |= rhs.m_mask;
        return *this;
    }

    constexpr flags<BitType>& operator&=(flags<BitType> rhs) noexcept {
        m_mask &= rhs.m_mask;
        return *this;
    }

    constexpr flags<BitType>& operator^=(flags<BitType> rhs) noexcept {
        m_mask ^= rhs.m_mask;
        return *this;
    }

    // cast operators
    explicit constexpr operator bool() const noexcept {
        return !!m_mask;
    }

    explicit constexpr operator MaskType() const noexcept {
        return m_mask;
    }

private:
    MaskType m_mask;
};

// bitwise operators
template<typename BitType>
constexpr flags<BitType> operator&(BitType bit, flags<BitType> const& flags) noexcept {
    return flags & bit;
}

template<typename BitType>
constexpr flags<BitType> operator|(BitType bit, flags<BitType> const& flags) noexcept {
    return flags | bit;
}

template<typename BitType>
constexpr flags<BitType> operator^(BitType bit, flags<BitType> const& flags) noexcept {
    return flags ^ bit;
}

} // namespace tst::core