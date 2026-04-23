#pragma once

#include <core/platform.h>
#include <cstddef>

namespace tst {

template<typename T>
TST_INLINE void hash_combine(size_t& seed, T value) noexcept {
    seed ^= static_cast<size_t>(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

} // namespace tst
