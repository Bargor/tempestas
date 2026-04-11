#pragma once

// Platform families
#define TST_PLATFORM_WINDOWS 0x00000001
#define TST_PLATFORM_LINUX 0x00000002
#define TST_PLATFORM_MACOS 0x00000004
#define TST_PLATFORM_UNIX 0x00000008
#define TST_PLATFORM_UNKNOWN 0x00000010

#ifdef _WIN32
#define TST_PLATFORM TST_PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#define TST_PLATFORM TST_PLATFORM_MACOS
#elif defined(__linux__)
#define TST_PLATFORM TST_PLATFORM_LINUX
#elif defined(__unix__)
#define TST_PLATFORM TST_PLATFORM_UNIX
#else
#define TST_PLATFORM TST_PLATFORM_UNKNOWN
#endif

// Compiler families only
#define TST_COMPILER_MSVC 0x00010000
#define TST_COMPILER_GCC 0x00020000
#define TST_COMPILER_CLANG 0x00040000
#define TST_COMPILER_OTHER 0x80000000

#ifdef _MSC_VER
#define TST_COMPILER TST_COMPILER_MSVC
#elif defined(__clang__)
#define TST_COMPILER TST_COMPILER_CLANG
#elif defined(__GNUC__)
#define TST_COMPILER TST_COMPILER_GCC
#else
#define TST_COMPILER TST_COMPILER_OTHER
#endif

// Backward-compat alias
#define TST_COMPILER_VC TST_COMPILER_MSVC

#if TST_COMPILER & TST_COMPILER_MSVC
#define TST_INLINE __forceinline
#define TST_NEVER_INLINE __declspec(noinline)
#define TST_CALL __vectorcall
#elif TST_COMPILER & TST_COMPILER_GCC
#define TST_INLINE inline __attribute__((__always_inline__))
#define TST_NEVER_INLINE __attribute__((__noinline__))
#ifdef TST_BUILD_32
#define TST_CALL __attribute__((fastcall))
#else
#define TST_CALL
#endif
#elif TST_COMPILER & TST_COMPILER_CLANG
#define TST_INLINE inline __attribute__((__always_inline__))
#define TST_NEVER_INLINE __attribute__((__noinline__))
#define TST_CALL
#else
#define TST_INLINE inline
#define TST_NEVER_INLINE
#define TST_CALL
#endif
