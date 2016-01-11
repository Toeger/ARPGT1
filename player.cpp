#include <array>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "player.h"
#include "utility.h"
#include "Physics/body.h"

Player Player::player;

Player::Player(sf::RenderWindow *window):
	camera(window)
{
}

void Player::set_window(sf::RenderWindow *window)
{
	camera.set_window(window);
}

void Player::center_camera()
{
	auto &t = get<Physical::Body>()->get_current_transformator();
	camera.set_position(t.vector);
	camera.set_rotation(-t.direction.to_degrees());
}

void Player::turn(float angle)
{
	(*get<Physical::Body>()) += Physical::Direction::from_radians(angle);
}

float degreeToRadians(float degree){
	return degree * M_PI / 180;
}
