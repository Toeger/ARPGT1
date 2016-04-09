#include "External/json.hpp"
#include "External/LuaContext.hpp"
#include "skill.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

std::vector<Skills::Skill_definition> Skills::load(std::istream &is, void (*setup_lua_environment)(LuaContext &))
{
	using json = nlohmann::json;
	std::vector<Skills::Skill_definition> retval;
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
				if (!property_value.is_string())
					throw std::runtime_error("value of skill property " + property_name + " must be of type string with a lua program as the content");
				skill.on_create = [code = property_value.get<std::string>(), setup_lua_environment](Skills::Skill_instance &skill){
					(void)skill;
					LuaContext context(false);
					setup_lua_environment(context);
					std::stringstream codestream(code);
					context.executeCode(codestream);
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

void Skills::setup_default_lua_environment(LuaContext &lua_context)
{
	//TODO: provide access to the caster, the target and various other stuff that a skill might need access to
	(void)lua_context;
}

Skills::Skill_instance Skills::Skill_definition::create()
{
	Skills::Skill_instance instance(*this);
	return instance;
}

Skills::Skill_instance::Skill_instance(const Skills::Skill_definition &skill_definition)
	:skill_definition(&skill_definition)
{}

void Skills::Skill_instance::on_create()
{
	skill_definition->on_create(*this);
}

void Skills::Skill_instance::on_tick()
{
	skill_definition->on_tick(*this);
}
