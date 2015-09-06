#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class Player
{
public:
    Player(b2World &world);
    void logicalUpdate();
    sf::CircleShape character;
    float speed = 100;
    b2Body *body;
};

#endif // PLAYER_H
