#pragma once
#include "platform.h"

#include <cstdio>
#include <cstdlib>

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
TST_INLINE void _assert(const char* expression, const char* file, int line) {
    fprintf(stderr, "Assertion '%s' failed, file '%s' line '%d'.", expression, file, line);
    debugBreak();
}

#undef assert

#ifdef NDEBUG
#define assert(EXPRESSION) ((void)0)
#else
#define assert(EXPRESSION) ((EXPRESSION) ? (void)0 : tst::_assert(#EXPRESSION, __FILE__, __LINE__))
#endif

} // namespace tst
