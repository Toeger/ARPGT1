#include "skill.h"
#include "ECS/common_components.h"
#include "External/LuaContext.hpp"
#include "External/json.hpp"
#include "Graphics/common_graphics_components.h"
#include "Physics/body.h"
#include "main.h"
#include "player.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

std::vector<Skills::Skill_instance> Skills::Skill_instance::instances;

std::vector<Skills::Skill_definition> Skills::load(std::istream &is, void (*setup_lua_environment)(LuaContext &)) {
	using json = nlohmann::json;
	std::vector<Skills::Skill_definition> retval;
	json j(is);
	using std::begin;
	using std::end;
	const auto join = [](const auto &container, const auto &separator) {
		std::string s = *begin(container);
		auto it = begin(container);
		for (++it; it != end(container); ++it) {
			s += separator;
			s += *it;
		}
		return s;
	};
	for (auto skill_it = begin(j); skill_it != end(j);
		 ++skill_it) { //can't use a range based for, because that only iterates over values, not keys like in std::map
		const auto &skill_name = skill_it.key();
		if (skill_name == "comment") //skip comments
			continue;
		retval.emplace_back();
		auto &skill = retval.back();
		skill.name = skill_name;
		for (auto property_it = begin(skill_it.value()); property_it != end(skill_it.value()); ++property_it) {
			const auto &property_name = property_it.key();
			const auto &property_value = property_it.value();
			if (property_name == "oncreate") {
				if (!property_value.is_string())
					throw std::runtime_error("value of skill property " + property_name + " must be of type string with a lua program as the content");
				skill.on_create = [ code = property_value.get_ref<const std::string &>(), setup_lua_environment ](Skills::Skill_instance & skill) {
					(void)skill;
					LuaContext context(false);
					setup_lua_environment(context);
					std::stringstream codestream(code);
					context.executeCode(codestream);
				};
			} else if (property_name == "onhit") {
				if (!property_value.is_string())
					throw std::runtime_error("value of skill property " + property_name + " must be of type string with a lua program as the content");
				skill.on_hit = [ code = property_value.get_ref<const std::string &>(), setup_lua_environment ](Skills::Skill_instance & skill) {
					(void)skill;
					LuaContext context(false);
					setup_lua_environment(context);
					std::stringstream codestream(code);
					context.executeCode(codestream);
				};
			} else if (property_name == "animation") {
				if (!property_value.is_string())
					throw std::runtime_error("value of skill property " + property_name + " must be of type string with a path to a 3D model");
				skill.animation = property_value.get_ref<const std::string &>();
			} else if (property_name == "texture") {
				if (!property_value.is_string())
					throw std::runtime_error("value of skill property " + property_name + " must be of type string with a path to a 3D model");
				skill.texture = property_value.get_ref<const std::string &>();
			} else if (property_name == "channeltime") {
				if (!property_value.is_number())
					throw std::runtime_error("value of skill property " + property_name + " must be of type number denoting a time in seconds");
				skill.channeltime = static_cast<Logical_time>(property_value.get<double>() * Config::lfps);
			} else if (property_name == "collision") {
				if (!property_value.is_array())
					throw std::runtime_error("value of skill property " + property_name + " must be of type array of strings of interaction types");
				for (auto &interaction : property_value) {
					if (!interaction.is_string())
						throw std::runtime_error("value of skill property " + property_name + " must be of type array of strings of interaction types");
					const auto &interaction_type_name = interaction.get_ref<const std::string &>();
					const auto collision_strings = {
						"allies", "enemies", "map", "self",
					};
					auto pos = lower_bound(begin(collision_strings), end(collision_strings), interaction_type_name);
					if (pos == end(collision_strings)) {
						//unknown interaction type
						throw std::runtime_error("interaction type must be one of [\"" + join(collision_strings, "\", \"") + "\"], found \"" +
												 interaction_type_name + "\"");
					}
					skill.affected[pos - begin(collision_strings)] = true;
				}
			} else if (property_name == "cooldown") {
				if (!property_value.is_number())
					throw std::runtime_error("value of skill property " + property_name + " must be of type number denoting a time in seconds");
				skill.cooldown = static_cast<Logical_time>(property_value.get<double>() * Config::lfps);
			} else if (property_name == "executiontime") {
				if (!property_value.is_number())
					throw std::runtime_error("value of skill property " + property_name + " must be of type number denoting a time in seconds");
				skill.executiontime = static_cast<Logical_time>(property_value.get<double>() * Config::lfps);
			} else if (property_name == "size") {
				if (!property_value.is_number())
					throw std::runtime_error("value of skill property " + property_name + " must be of type number denoting the size of the skill");
				skill.size = property_value.get<float>();
			} else if (property_name == "speed") {
				if (!property_value.is_number())
					throw std::runtime_error("value of skill property " + property_name + " must be of type number denoting the size of the skill");
				skill.speed = property_value.get<float>();
			} else if (property_name == "type") {
				if (!property_value.is_string())
					throw std::runtime_error("value of skill property " + property_name + " must be of type string denoting the type of the skill");
				const auto type_strings = {
					"aura", "instant", "invalid", "projectile",
				};
				auto pos = lower_bound(begin(type_strings), end(type_strings), property_value.get_ref<const std::string &>());
				if (pos == end(type_strings)) {
					throw std::runtime_error("value of skill property " + property_name + " must be one of [\"" + join(type_strings, "\", \"") + "]");
				}
				skill.type = static_cast<Skills::Type>(pos - begin(type_strings));
			} else {
				//unknown property
				throw std::runtime_error("unknown skill property: \"" + property_name + "\" in skill \"" + skill_name + "\"");
			}
		}
	}
	return retval;
}

void Skills::setup_default_lua_environment(LuaContext &lua_context) {
	//TODO: provide access to the caster, the target and various other stuff that a skill might need access to
	(void)lua_context;
}

Skills::Skill_instance Skills::Skill_definition::create() {
	Skills::Skill_instance instance(*this);
	switch (type) {
	case Type::aura:
		break;
	case Type::instant:
		break;
	case Type::projectile: {
		const auto player_trans = Player::player.get<Physical::DynamicBody<Physical::Circle>>()->get_current_transformator();
		auto &pos = player_trans.vector;
		const auto block_size = Map::current_map->get_block_size();
		auto xpos = Map::current_map->get_width() - 1 - pos.x / block_size;
		auto ypos = pos.y / block_size;
		if (texture.size()){
			instance.emplace<Common_components::Animated_model>(*Window::current_window, animation, texture).set_position(xpos, 0, ypos);
		}
		else{
			instance.emplace<Common_components::Animated_model>(*Window::current_window, animation).set_position(xpos, 0, ypos);
		}
		instance.emplace<Common_components::Speed>(speed);
		auto &projectile_body = instance.emplace<Physical::DynamicBody<Physical::Circle>>(size);
		projectile_body.force_move(player_trans);
		break;
	} break;
	case Type::invalid:
	case Type::size:
		throw std::runtime_error("invalid skill type");
	}

	return instance;
}

Skills::Skill_instance::Skill_instance(const Skills::Skill_definition &skill_definition)
	: skill_definition(&skill_definition) {}

void Skills::Skill_instance::on_create() {
	skill_definition->on_create(*this);
}

void Skills::Skill_instance::on_tick() {
	skill_definition->on_tick(*this);
}
