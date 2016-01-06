#ifndef TESTS_UTILITY_H
#define TESTS_UTILITY_H

#include <cassert>
#include <cmath>
#include <type_traits>

template <class T, class U>
std::enable_if_t<!std::is_floating_point<T>::value && !std::is_floating_point<U>::value>
assert_equal(const T &a, const U &b){
	//this function exists for debugging purposes
	//assert(foo() == bar()); will not let me examine the return values of foo() and bar(),
	//while this function captures references to the return values
	assert(a == b);
}

//special cases for float comparisons
template <class T, class U>
std::enable_if_t<std::is_floating_point<T>::value || std::is_floating_point<U>::value>
assert_equal(const T &a, const U &b){
	if (std::abs(b) < 1){
		assert(a - b < 0.00005);
	}
	else{
		assert(std::abs(a / b) - 1 < 0.000005);
	}
}

#endif // TESTS_UTILITY_H
