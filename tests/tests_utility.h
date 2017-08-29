#ifndef TESTS_UTILITY_H
#define TESTS_UTILITY_H

#include "utility/asserts.h"

#include <cmath>
#include <type_traits>

template <class T, class U>
void assert_equal(const T &a, const U &b) {
	//this function exists for debugging purposes
	//assert(foo() == bar()); will not let me examine the return values of foo() and bar(),
	//while this function captures references to the return values
	if constexpr (!std::is_floating_point<T>::value && !std::is_floating_point<U>::value) {
		assert_fast(a == b);
	} else { //special cases for float comparisons
		if (std::abs(b) < 1) {
			assert_fast(a - b < 0.0005);
		} else {
			assert_fast(std::abs(a / b) - 1 < 0.000005);
		}
	}
}

#endif // TESTS_UTILITY_H
