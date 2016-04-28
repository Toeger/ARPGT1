#ifndef SKILL_H
#define SKILL_H

#include "ECS/entity.h"

#include <bitset>
#include <functional>
#include <istream>
#include <string>
#include <vector>

class LuaContext;

namespace Skills {
	enum class Type { aura, instant, invalid, projectile, size };
	enum class Collisions { allies, enemies, map, self, size };

	//skill concepts:
	/* Target - Either the target entity or an area
	 * Effect - What happens when the skill hits
	 * Generic stats - Cooldown, channeltime, executiontime, cost, ...
	 * Affected - List of types of objects affected, could be enemies, allies, other skills
	 * Functions to get data, because the channeltime may depend on buffs or effects
	 * Graphics - Data such as size and animation speed for the graphics to use
	 * Need to keep the skill description separate from modifiers and instances of skills
	 */
	using Logical_time = int; //time in logical frames for things to happen

	struct Skill_instance;

	struct Skill_definition {
		//the definition of a skill including base stats, descriptions and behaviors
		std::string name;
		std::string animation;
		std::string texture;
		float size = 0.f;
		float speed = 0.f;
		Logical_time channeltime{0};
		Logical_time executiontime{0};
		Logical_time cooldown{0};
		ECS::Entity_handle caster;
		ECS::Entity_handle target;
		std::bitset<static_cast<std::size_t>(Collisions::size)> affected;
		std::function<void(Skill_instance &)> on_create = [](Skill_instance &) {}; //function to be called when an instance of the skill is created
		std::function<void(Skill_instance &)> on_tick = [](Skill_instance &) {};   //function to be called every tick
		std::function<void(Skill_instance &)> on_hit = [](Skill_instance &) {};    //function to be called every tick
		Type type = Type::invalid;

		Skill_instance create();
	};

	struct Modifier;

	struct Skill_instance : ECS::Entity {
		//instances of skills with specific data such as the current modifiers, position, direction and state
		Skill_instance(const Skill_definition &skill_definition);
		Skill_instance(Skill_instance &&) = default;
		Skill_instance &operator=(Skill_instance &&) = default;
		const Skill_definition *skill_definition = nullptr;
		std::vector<Skills::Modifier> modifiers;
		void on_create();
		void on_tick();
	};

	struct Modifier {
		//modifies a skill, contains the name of the modifier, the priority (in what order modifiers are to be applied) and a function to modify a Skill
		enum class Priority : char { First, Before, Normal, After, Last };
		template <class String, class Function>
		Modifier(String &&name, Function &&function, Priority priority = Priority::Normal)
			: name(std::forward<String>(name))
			, apply(std::forward<Function>(function))
			, priority(priority) {}

		std::string name;
		std::function<void(Skills::Skill_instance)> apply;
		Priority priority;
	};
	inline bool operator<(const Modifier &lhs, const Modifier &rhs) {
		return std::tie(lhs.priority, lhs.name) < std::tie(rhs.priority, rhs.name);
	}

	void setup_default_lua_environment(LuaContext &lua_context);

	std::vector<Skill_definition> load(std::istream &is, void (*setup_lua_environment)(LuaContext &) = &setup_default_lua_environment);
}

#endif // SKILL_H
