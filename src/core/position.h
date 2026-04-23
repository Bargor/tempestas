#pragma once

#include "util/hash.h"

#include <cstddef>
#include <cstdint>
#include <functional>

namespace tst {
namespace core {

    template<typename T>
    struct position {
        T x;
        T y;

        bool operator==(const position& other) const noexcept;
    };

    template<typename T>
    bool position<T>::operator==(const position& other) const noexcept {
        return x == other.x && y == other.y;
    }
} // namespace core
} // namespace tst

namespace std {

template<typename T>
struct hash<tst::core::position<T>> {
    size_t operator()(const tst::core::position<T>& position) const noexcept {
        size_t seed = 0;
        tst::hash_combine(seed, hash<T>{}(position.x));
        tst::hash_combine(seed, hash<T>{}(position.y));

        return seed;
    }
};

} // namespace std
