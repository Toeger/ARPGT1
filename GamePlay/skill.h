#ifndef SKILL_H
#define SKILL_H

#include "ECS/entity.h"

#include <bitset>
#include <functional>
#include <istream>
#include <string>
#include <vector>

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
		std::function<void(Skill &)> create = [](Skill &){}; //function to be called when an instance of the skill is created
		std::function<void(Skill &)> tick = [](Skill &){}; //function to be called every tick
	};

	std::vector<Skill> load(std::istream &is);
}

#endif // SKILL_H
