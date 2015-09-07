#ifndef PRACTICEDUMMY_H
#define PRACTICEDUMMY_H

#include <SFML/Graphics.hpp>
#include "physicalcircle.h"

class PracticeDummy
{
public:
    PracticeDummy(b2World &world);
    void logicalUpdate();
    sf::CircleShape character;
    PhysicalCircle pc;
};

#endif // PRACTICEDUMMY_H
