#include <array>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "player.h"
#include "conversions.h"
#include "sensor.h"
#include "utility.h"
#include "hittable.h"
#include "enemylist.h"
#include "components.h"

Player Player::player;

Player::Player(sf::RenderWindow *window):
	camera(window)
{
}

void Player::set_window(sf::RenderWindow *window)
{
	camera.set_window(window);
}

float degreeToRadians(float degree){
	return degree * M_PI / 180;
}
