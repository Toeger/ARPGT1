#ifndef TESTS_UTILITY_H
#define TESTS_UTILITY_H

#include <cassert>

inline void assert_equal(float a, float b){
	assert(a - b < 0.0005);
}

inline void assert_equal(int a, float b){
	assert_equal(a * 1.f, b);
}

inline void assert_equal(float a, int b){
	assert_equal(a, b * 1.f);
}

template <class T, class U>
void assert_equal(const T &a, const U &b){
	//this function exists for debugging purposes
	//assert(foo() == bar()); will not let me examine the return values of foo() and bar(),
	//while this function captures references to the return values
	assert(a == b);
}

#endif // TESTS_UTILITY_H
