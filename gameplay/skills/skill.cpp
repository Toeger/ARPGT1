#include "skill.h"
#include "ecs/common_components.h"
#include "graphics/common_graphics_components.h"
#include "main.h"
#include "physics/body.h"
#include "player.h"

#include <external/LuaContext.hpp>
#include <external/json.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
	constexpr Skills::Collision get_next_collision(Skills::Collision collision) {
		switch (collision) {
			case Skills::Collision::allies:
				return Skills::Collision::enemies;
			case Skills::Collision::enemies:
				return Skills::Collision::map;
			case Skills::Collision::map:
				return Skills::Collision::self;
			case Skills::Collision::self:
				return Skills::Collision::size;
			case Skills::Collision::size:
				return Skills::Collision::size;
				//should an enum value not be handled clang will complain, so we don't even bother with a default value
		}
	}

	template <Skills::Collision c>
	bool _collides_with(ECS::Entity_handle eh, const Skills::Collision_type_tags tags);
	template <>
	bool _collides_with<Skills::Collision::size>(ECS::Entity_handle /*unused*/, const Skills::Collision_type_tags /*unused*/) {
		return false;
	}
	template <Skills::Collision c>
	bool _collides_with(ECS::Entity_handle eh, const Skills::Collision_type_tags tags) {
		if (tags[static_cast<std::size_t>(c)] && eh.get<Skills::Collision_tag<c>>()) {
			return true;
		}
		return _collides_with<get_next_collision(c)>(eh, tags);
	}
} // namespace

bool Skills::collides_with(ECS::Entity_handle eh, const Skills::Collision_type_tags tags) {
	return _collides_with<Collision::allies>(eh, tags);
}

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
		if (skill_name == "comment") { //skip comments
			continue;
		}
		retval.emplace_back();
		auto &skill = retval.back();
		skill.name = skill_name;
		for (auto property_it = begin(skill_it.value()); property_it != end(skill_it.value()); ++property_it) {
			const auto &property_name = property_it.key();
			const auto &property_value = property_it.value();
			if (property_name == "oncreate") {
				if (!property_value.is_string()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type string with a lua program as the content");
				}
				skill.on_create = [ code = property_value.get_ref<const std::string &>(), setup_lua_environment ](Skills::Skill_instance & skill) {
					(void)skill;
					LuaContext context(false);
					setup_lua_environment(context);
					std::stringstream codestream(code);
					context.executeCode(codestream);
				};
			} else if (property_name == "onhit") {
				if (!property_value.is_string()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type string with a lua program as the content");
				}
				skill.on_hit = [ code = property_value.get_ref<const std::string &>(), setup_lua_environment ](Skills::Skill_instance & skill) {
					(void)skill;
					LuaContext context(false);
					setup_lua_environment(context);
					std::stringstream codestream(code);
					context.executeCode(codestream);
				};
			} else if (property_name == "animation") {
				if (!property_value.is_string()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type string with a path to a 3D model");
				}
				skill.animation = ARTDIR "/" + property_value.get_ref<const std::string &>();
			} else if (property_name == "texture") {
				if (!property_value.is_string()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type string with a path to a 3D model");
				}
				skill.texture = ARTDIR "/" + property_value.get_ref<const std::string &>();
			} else if (property_name == "channeltime") {
				if (!property_value.is_number()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type number denoting a time in seconds");
				}
				skill.channeltime = static_cast<Logical_time>(property_value.get<double>() * Config::lfps);
			} else if (property_name == "collision") {
				if (!property_value.is_array()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type array of strings of interaction types");
				}
				for (auto &interaction : property_value) {
					if (!interaction.is_string()) {
						throw std::runtime_error("value of skill property " + property_name + " must be of type array of strings of interaction types");
					}
					const auto &interaction_type_name = interaction.get_ref<const std::string &>();
					const auto collision_strings = {
						"allies",
						"enemies",
						"map",
						"self",
					};
					auto pos = lower_bound(begin(collision_strings), end(collision_strings), interaction_type_name);
					if (pos == end(collision_strings)) {
						//unknown interaction type
						throw std::runtime_error("interaction type must be one of [\"" + join(collision_strings, "\", \"") + "\"], found \"" +
												 interaction_type_name + "\"");
					}
					skill.collision_types[pos - begin(collision_strings)] = true;
				}
			} else if (property_name == "cooldown") {
				if (!property_value.is_number()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type number denoting a time in seconds");
				}
				skill.cooldown = static_cast<Logical_time>(property_value.get<double>() * Config::lfps);
			} else if (property_name == "executiontime") {
				if (!property_value.is_number()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type number denoting a time in seconds");
				}
				skill.executiontime = static_cast<Logical_time>(property_value.get<double>() * Config::lfps);
			} else if (property_name == "size") {
				if (!property_value.is_number()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type number denoting the size of the skill");
				}
				skill.size = property_value.get<float>();
			} else if (property_name == "speed") {
				if (!property_value.is_number()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type number denoting the size of the skill");
				}
				skill.speed = property_value.get<float>();
			} else if (property_name == "type") {
				if (!property_value.is_string()) {
					throw std::runtime_error("value of skill property " + property_name + " must be of type string denoting the type of the skill");
				}
				const auto type_strings = {
					"aura",
					"instant",
					"invalid",
					"projectile",
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
	instance.collision_types = collision_types;
	switch (type) {
		case Type::aura:
			break;
		case Type::instant:
			break;
		case Type::projectile: {
			const auto player_body = Player::player.get<Physical::Dynamic_body<Physical::Circle>>();
			const auto player_trans = player_body->get_current_transformator();
			const auto pos = Map::current_map->to_world_coords(player_trans.vector);
			if (texture.empty()) {
				instance.emplace<Common_components::Animated_model>(*Window::current_window, animation).set_position(pos);
			} else {
				instance.emplace<Common_components::Animated_model>(*Window::current_window, animation, texture).set_position(pos);
			}
			instance.emplace<Common_components::Speed>(speed);
			const auto player_radius = player_body->get_shape().radius;
			auto &projectile_body = instance.emplace<Physical::Dynamic_body<Physical::Circle>>(size); //after this line player_body is invalid
			projectile_body.force_move(player_trans + Physical::Vector{0, 2 * player_radius + size});
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
