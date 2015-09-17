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

Player::Player(sf::RenderWindow *window):
    camera(window)
{

    Components::CircleShape character(50);
    character.setFillColor({50, 100, 200});
    add(std::move(character));
    //character.setPosition(400, 300);
    //UpdateList::add([this](){this->logicalUpdate();});
}

float degreeToRadians(float degree){
    return degree * M_PI / 180;
}
