#include "skill.h"
#include "External/json.hpp"

#include <iostream>

std::vector<Skills::Skill> Skills::load(std::istream &is)
{
	using json = nlohmann::json;
	std::vector<Skills::Skill> retval;
	json j(is);
	for (auto &e : j){
		std::cerr << e << '\n';
	}
	return retval;
}
