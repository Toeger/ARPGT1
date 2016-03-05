#ifndef PLAYER_H
#define PLAYER_H

#include "ECS/entity.h"
#include "Physics/physics_utility.h"

class Player : public ECS::Entity
{
public:
	Physical::Vector move_offset;
	Physical::Direction turn_speed = Physical::Direction::from_degrees(4);
	Physical::Direction turn_offset;
	void turn(float angle);
	static Player player;
};

#endif // PLAYER_H
