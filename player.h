#ifndef PLAYER_H
#define PLAYER_H

#include "ECS/entity.h"
#include "Physics/physics_utility.h"

class Player : public ECS::Entity
{
public:
	static Player player;
};

#endif // PLAYER_H
