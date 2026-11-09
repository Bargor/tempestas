#pragma once

#include <type_traits>

namespace tst::core {

template<typename FlagBitsType>
struct flag_traits {
    enum { eAll_flags = 0 };
};

template<typename BitType>
class flags {
public:
    using MaskType = typename std::underlying_type<BitType>::type;

    // constructors
    constexpr flags() noexcept : m_mask(0) {
    }

    constexpr flags(BitType bit) noexcept : m_mask(static_cast<MaskType>(bit)) {
    }

    constexpr flags(flags<BitType> const& rhs) noexcept : m_mask(rhs.m_mask) {
    }

    constexpr explicit flags(MaskType flags) noexcept : m_mask(flags) {
    }

public:
    auto operator<=>(flags<BitType> const&) const = default;

    constexpr bool operator!() const noexcept {
        return !m_mask;
    }

    constexpr flags<BitType> operator&(flags<BitType> rhs) const noexcept {
        return flags<BitType>(m_mask & rhs.m_mask);
    }

    constexpr flags<BitType> operator|(flags<BitType> rhs) const noexcept {
        return flags<BitType>(m_mask | rhs.m_mask);
    }

    constexpr flags<BitType> operator^(flags<BitType> rhs) const noexcept {
        return flags<BitType>(m_mask ^ rhs.m_mask);
    }

    constexpr flags<BitType> operator~() const noexcept {
        return flags<BitType>(m_mask ^ flag_traits<BitType>::eAll_flags);
    }

    constexpr flags<BitType>& operator=(flags<BitType> rhs) noexcept {
        m_mask = rhs.m_mask;
        return *this;
    }

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