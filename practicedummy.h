#ifndef PRACTICEDUMMY_H
#define PRACTICEDUMMY_H

#include <SFML/Graphics.hpp>
#include "physicalcircle.h"
#include "weapon.h"

class PracticeDummy
{
public:
    PracticeDummy(b2World &world);
    void logicalUpdate();
    sf::CircleShape character;
    PhysicalCircle pc;
    void take_hit(Weapon &weapon);
};

#endif // PRACTICEDUMMY_H
