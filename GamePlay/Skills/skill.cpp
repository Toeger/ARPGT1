#include "External/json.hpp"
#include "skill.h"

#include <iostream>
#include <luabind/luabind.hpp>
#include <stdexcept>
#include <tolua++.h>

#include "luaskillbindings.h" //this header is auto generated and missing an #include, so we put it after #including the right thing

template <class Function, class... Args>
static void call_lua_function(Function function, lua_State *state, Args... args){
	auto error = function(state, args...);
	if (error){
		const char *lua_error = lua_tostring(state, error);
		if (!lua_error)
			lua_error = "unknown";
		auto error_message = std::string("lua error: ") + lua_error;
		throw std::runtime_error(std::move(error_message));
	}
}

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
				if (!property_value.is_string())
					throw std::runtime_error("value of skill property " + property_name + " must be of type string with a lua program as the content");
				skill.oncreate = [code = property_value.get<std::string>()](Skills::Skill &skill){
					(void)skill;
					//todo: execute the lua code that was captured
					auto state = lua_open();
					//call_lua_function(&tolua_luaskillbindings_open, state);
					call_lua_function(&luaL_loadstring, state, code.c_str());
					call_lua_function(&lua_pcall, state, 0, 0, 0);
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
