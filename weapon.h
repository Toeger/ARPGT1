#ifndef WEAPON_H
#define WEAPON_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class Weapon
{
public:
    Weapon(b2Body *body);
    b2Fixture *fixture;
    sf::ConvexShape drawShape;
    void logicalUpdate();
};

#endif // WEAPON_H
