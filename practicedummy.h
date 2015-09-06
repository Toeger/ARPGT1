#ifndef PRACTICEDUMMY_H
#define PRACTICEDUMMY_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class PracticeDummy
{
public:
    PracticeDummy(b2World &world);
    void logicalUpdate();
    sf::CircleShape character;
    b2Body *body;
};

#endif // PRACTICEDUMMY_H
