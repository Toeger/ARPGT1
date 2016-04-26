#ifndef PLAYER_H
#define PLAYER_H

#include "ECS/entity.h"
#include "GamePlay/Skills/skill.h"
#include "Physics/physics_utility.h"

#include <vector>

class Player : public ECS::Entity {
		public:
	static Player player;
	std::vector<Skills::Skill_definition> skills;
};

#endif // PLAYER_H
