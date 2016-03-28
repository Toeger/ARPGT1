#ifndef LUASKILLS_H
#define LUASKILLS_H

#include <string>
#include <iostream>

inline void print(const std::string &string){
	std::cerr << "From LUA: " << string << "\n";
}

#endif // LUASKILLS_H
