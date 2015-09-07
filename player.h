#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "physicalcircle.h"

class Player
{
public:
    Player(b2World &world);
    void logicalUpdate();
    sf::CircleShape character;
    float speed = 0.6;
    PhysicalCircle pc;
private:
    b2World *get_world();
};

#endif // PLAYER_H
