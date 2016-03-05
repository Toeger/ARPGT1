#include "ECS/utility.h"
#include "Physics/body.h"
#include "player.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>

Player Player::player;

void Player::turn(float angle)
{
	(*get<Physical::DynamicBody<Physical::Circle>>()) += Physical::Direction::from_radians(angle);
}

float degreeToRadians(float degree){
	return degree * M_PI / 180;
}
