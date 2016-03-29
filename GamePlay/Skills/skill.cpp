#include "External/json.hpp"
#include "skill.h"

#include <iostream>

std::vector<Skills::Skill> Skills::load(std::istream &is)
{
	using json = nlohmann::json;
	std::vector<Skills::Skill> retval;
	json j(is);
	using std::begin;
	using std::end;
	for (auto skill_it = begin(j); skill_it != end(j); ++skill_it){ //can't use a range based for, because that only iterates over values, not keys like in std::map
		const auto &skill_name = skill_it.key();
		if (skill_name == "comment") //skip comments
			continue;
		retval.emplace_back();
		auto &skill = retval.back();
		skill.name = skill_name;
		for (auto property_it = begin(skill_it.value()); property_it != end(skill_it.value()); ++property_it){
			const auto &property_name = property_it.key();
			const auto &property_value = property_it.value();
			if (property_name == "oncreate"){
				skill.oncreate = [code = property_value](Skills::Skill &skill){
					(void)skill;
					//todo: execute the lua code that was captured
				};
			}
			else{
				//unknown property
				throw std::runtime_error("unknown skill property: " + property_name + " in skill " + skill_name);
			}
		}
	}
	return retval;
}
