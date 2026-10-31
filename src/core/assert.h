#pragma once
#include "platform.h"

#include <cstdio>
#include <cstdlib>
#include <fmt/printf.h>

#if TST_COMPILER & TST_COMPILER_VC
#define debugBreak() __debugbreak()
#elif TST_COMPILER & TST_COMPILER_GCC
#include <signal.h>
#define debugBreak() raise(SIGINT);
#elif TST_COMPILER & TST_COMPILER_CLANG
#include <signal.h>
#define debugBreak() raise(SIGINT);
#endif

namespace tst {
TST_INLINE void assert_failed(const char* expression, const char* file, int line) {
    fmt::fprintf(stderr, "Assertion '%s' failed, file '%s' line '%d'.", expression, file, line);
    debugBreak();
}

#undef assert

#ifdef NDEBUG
#define assert(EXPRESSION) ((void)0)
#else
#define assert(EXPRESSION) ((EXPRESSION) ? (void)0 : tst::assert_failed(#EXPRESSION, __FILE__, __LINE__))
#endif

#define hard_assert(EXPRESSION, MESSAGE, ...) \
    ((EXPRESSION) ? (void)0 :                 \
                    tst::hard_assert_failed(#EXPRESSION, fmt::format(MESSAGE __VA_OPT__(, ) __VA_ARGS__).c_str()))

TST_INLINE void hard_assert_failed(const char* expression, const char* message) {
    fmt::fprintf(stderr, "Assertion '%s' failed. Message: %s", expression, message);
    std::abort();
}

} // namespace tst
