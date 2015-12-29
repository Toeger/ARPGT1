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
	auto &t = get<Physical::Body>()->transformator;
	camera.set_position(t.get_translation());
	camera.set_rotation(t.get_rotation().to_angle());
}

void Player::turn(float angle)
{
	auto &t = get<Physical::Body>()->transformator;
	t = Physical::Transformator::get_rotation_matrix(t.get_translation(), {cos(angle), sin(angle)}) * t;
}

float degreeToRadians(float degree){
	return degree * M_PI / 180;
}
