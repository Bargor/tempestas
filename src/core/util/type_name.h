#pragma once

#include <string_view>

namespace tst::core {

template<typename T>
consteval std::string_view type_name() {
#if defined(__clang__) || defined(__GNUC__)
    return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
    return __FUNCSIG__;
#else
#error Unsupported compiler: provide type_name() for this compiler
#endif
}

} // namespace tst::core