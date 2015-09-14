#ifndef PRACTICEDUMMY_H
#define PRACTICEDUMMY_H

#include <SFML/Graphics.hpp>
#include "weapon.h"

class PracticeDummy
{
public:
    PracticeDummy();
    void logicalUpdate();
    sf::CircleShape character;
};

#endif // PRACTICEDUMMY_H
