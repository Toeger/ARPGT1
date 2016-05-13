#ifndef PLAYER_H
#define PLAYER_H

#include "ecs/entity.h"
#include "gameplay/skills/skill.h"
#include "physics/physics_utility.h"

#include <vector>

class Player : public ECS::Entity {
	public:
	static Player player;
	std::vector<Skills::Skill_definition> skills;
};

#endif // PLAYER_H
