#ifndef ASSERTS_H
#define ASSERTS_H

#include <cassert>

#define ASSERT_LEVEL_NONE 0  //disable all asserts
#define ASSERT_LEVEL_FAST 1  //enable asserts that produce less than ~5% slowdown
#define ASSERT_LEVEL_DEBUG 2 //enable asserts that produce less than ~50% slowdown
#define ASSERT_LEVEL_ALL 3   //enable all asserts, even if they take a long time

#ifndef ASSERT_LEVEL
#define ASSERT_LEVEL ASSERT_LEVEL_ALL //default assert level, #define ASSERT_LEVEL to whichever level you want before #including asserts.h
#endif

namespace { //prevent ODR violations
#if ASSERT_LEVEL > 0
	template <class T>
	inline void assert_fast(T &&t) {
		if (!t) {
#ifdef _NDEBUG
			__builtin_unreachable();
#else
			assert(!"assertion failure");
#endif
		}
	}
#else
	template <class T>
	inline void assert_fast(T &&) {}
#endif

#if ASSERT_LEVEL > 1
	template <class T>
	inline void assert_debug(T &&t) {
		if (!t) {
#ifdef _NDEBUG
			__builtin_unreachable();
#else
			assert(!"assertion failure");
#endif
		}
	}
#else
	template <class T>
	inline void assert_debug(T &&) {}
#endif

#if ASSERT_LEVEL > 2
	template <class T>
	inline void assert_all(T &&t) {
		if (!t) {
#ifdef _NDEBUG
			__builtin_unreachable();
#else
			assert(!"assertion failure");
#endif
		}
	}
#else
	template <class T>
	inline void assert_all(T &&) {}
#endif
}
#endif // ASSERTS_H
