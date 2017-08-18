#ifndef MAKE_FUNCTION
#define MAKE_FUNCTION

#include <functional>
#include <iostream>
#include <utility>

// For generic types that are functors, delegate to its 'operator()'
template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())> {};

// for pointers to member function
template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const> {
	enum { arity = sizeof...(Args) };
	typedef std::function<ReturnType(Args...)> f_type;
};

// for pointers to member function
template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...)> {
	enum { arity = sizeof...(Args) };
	typedef std::function<ReturnType(Args...)> f_type;
};

// for function pointers
template <typename ReturnType, typename... Args>
struct function_traits<ReturnType (*)(Args...)> {
	enum { arity = sizeof...(Args) };
	typedef std::function<ReturnType(Args...)> f_type;
};

//handles bind & multiple function call operator()'s
template <typename ReturnType, typename... Args, class T>
auto make_function(T &&t) -> std::function<decltype(ReturnType(t(std::declval<Args>()...)))(Args...)> {
	return {std::forward<T>(t)};
}

//handles explicit overloads
template <typename ReturnType, typename... Args>
auto make_function(ReturnType (*p)(Args...)) -> std::function<ReturnType(Args...)> {
	return {p};
}

//handles explicit overloads
template <typename ReturnType, typename... Args, typename ClassType>
auto make_function(ReturnType (ClassType::*p)(Args...)) -> std::function<ReturnType(Args...)> {
	return {p};
}

#endif // MAKE_FUNCTION
