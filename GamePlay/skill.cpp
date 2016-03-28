#include "skill.h"
#include "External/json.hpp"

#include <iostream>

std::vector<Skills::Skill> Skills::load(std::istream &is)
{
	using json = nlohmann::json;
	std::vector<Skills::Skill> retval;
	json j(is);
	using std::begin;
	using std::end;
	for (auto it = begin(j); it != end(j); ++it){ //can't use a range based for, because that only iterates over values, not keys like in std::map
		if (it.key() == "comment") //skip comments
			continue;
		retval.emplace_back();
		auto &skill = retval.back();
		skill.name = it.key();
		std::cerr << it.key() << ':' << it.value() << '\n';
	}
	return retval;
}
