#ifndef PRACTICEDUMMY_H
#define PRACTICEDUMMY_H

#include <SFML/Graphics.hpp>
#include "weapon.h"
#include "entity.h"

class PracticeDummy : public Entity
{
public:
    PracticeDummy();
    sf::CircleShape character;
};

#endif // PRACTICEDUMMY_H
