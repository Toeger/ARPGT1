#include <array>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "player.h"
#include "ECS/utility.h"
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
	auto &t = get<Physical::DynamicBody<Physical::Circle>>()->get_current_transformator();
	camera.set_rotation(-t.direction.to_degrees());
	camera.set_position((t + Physical::Vector(0, camera.get_screen_size().y * 1/6.f)).vector);
}

void Player::turn(float angle)
{
	(*get<Physical::DynamicBody<Physical::Circle>>()) += Physical::Direction::from_radians(angle);
}

float degreeToRadians(float degree){
	return degree * M_PI / 180;
}
