#ifndef SKILL_H
#define SKILL_H

#include "ECS/entity.h"

#include <bitset>
#include <functional>
#include <istream>
#include <string>
#include <vector>

class LuaContext;

namespace Skills
{
	enum class Type{
		projectile,
		aura,
		instant,
	};
	enum Collisions{
		enemies,
		allies,
		self,
		map,
		size
	};

	//skill concepts:
	/* Target - Either the target entity or an area
	 * Effect - What happens when the skill hits
	 * Generic stats - Cooldown, channeltime, executiontime, cost, ...
	 * Affected - List of types of objects affected, could be enemies, allies, other skills
	 * Functions to get data, because the channeltime may depend on buffs or effects
	 * Graphics - Data such as size and animation speed for the graphics to use
	 */
	using Logical_time = int; //time in logical frames for things to happen

	struct Skill{
		std::string name;
		Logical_time channeltime{0};
		Logical_time executiontime{0};
		Logical_time cooldown{0};
		ECS::Entity_handle caster;
		ECS::Entity_handle target;
		std::vector<Collisions> affected;
		std::function<void(Skill &)> oncreate; //function to be called when an instance of the skill is created
		std::function<void(Skill &)> ontick; //function to be called every tick
	};

	struct Modifier{
		//modifies a skill, contains the name of the modifier, the priority (in what order modifiers are to be applied) and a function to modify a Skill
		enum class Priority : char{
			First,
			Before,
			Normal,
			After,
			Last
		};
		template<class String, class Function>
		Modifier(String &&name, Function &&function, Priority priority = Priority::Normal)
			:name(std::forward<String>(name))
			,apply(std::forward<Function>(function))
			,priority(priority)
		{}

		std::string name;
		std::function<void(Skills::Skill)> apply;
		Priority priority;
	};
	inline bool operator <(const Modifier &lhs, const Modifier &rhs){
		return std::tie(lhs.priority, lhs.name) < std::tie(rhs.priority, rhs.name);
	}

	void setup_default_lua_environment(LuaContext &lua_context);

	std::vector<Skill> load(std::istream &is, void (*setup_lua_environment)(LuaContext &) = &setup_default_lua_environment);
}

#endif // SKILL_H
