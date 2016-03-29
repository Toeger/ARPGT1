#ifndef LUASKILLS_H
#define LUASKILLS_H

#include <functional>
#include <iostream>
#include <string>

inline void print(const std::string &string){
	std::cerr << "From LUA: " << string << "\n";
}

extern std::function<void()> test_function;

inline void test(){
	test_function();
}

#endif // LUASKILLS_H
