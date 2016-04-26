#ifndef ASSERTS_H
#define ASSERTS_H

#include <cassert>

#define ASSERT_LEVEL_NONE 0  //disable all asserts
#define ASSERT_LEVEL_FAST 1  //enable asserts that produce less than ~5% slowdown
#define ASSERT_LEVEL_DEBUG 2 //enable asserts that produce less than ~50% slowdown
#define ASSERT_LEVEL_ALL 3   //enable all asserts, even if they take a long time

#ifndef ASSERT_LEVEL
#define ASSERT_LEVEL ASSERT_LEVEL_DEBUG //default assert level, #define ASSERT_LEVEL to whichever level you want before #including asserts.h
#endif

#if ASSERT_LEVEL > 0
template <class T>
inline void assert_fast(T &&t) {
	if (!t) {
#ifdef _NDEBUG //make sure assert is activated even during a release build
#undef _NDEBUG
		assert(!"assertion failure");
#define _NDEBUG
#else
		assert(!"assertion failure");
#endif
	}
}
#else
inline void assert_fast(bool) {}
#endif

#if ASSERT_LEVEL > 1
template <class T>
inline void assert_debug(T &&t) {
	if (!t) {
#ifdef _NDEBUG //make sure assert is activated even during a release build
#undef _NDEBUG
		assert(!"assertion failure");
#define _NDEBUG
#else
		assert(!"assertion failure");
#endif
	}
}
#else
inline void assert_debug(bool) {}
#endif

#if ASSERT_LEVEL > 2
template <class T>
inline void assert_all(T &&t) {
	if (!t) {
#ifdef _NDEBUG //make sure assert is activated even during a release build
#undef _NDEBUG
		assert(!"assertion failure");
#define _NDEBUG
#else
		assert(!"assertion failure");
#endif
	}
}
#else
inline void assert_all(bool) {}
#endif

#endif // ASSERTS_H
