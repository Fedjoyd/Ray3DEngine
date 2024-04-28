#pragma once
#ifndef _WIN32
#include <signal.h> // SIGTRAP
#endif

#if !defined(_WIN32) && !defined(__linux__)
#error Platform not supported
#endif

#ifndef MYTH_ASSERT
#if defined _MSC_VER && !defined __clang__
#define MYTH_ASSERT(x) __assume(x)
#else
#define MYTH_ASSERT(x) { false ? (void)(x) : (void)0; }
#endif
#endif