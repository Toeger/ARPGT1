#ifndef PRACTICEDUMMY_H
#define PRACTICEDUMMY_H

#include "ECS/entity.h"

#include <SFML/Graphics.hpp>

class PracticeDummy : public ECS::Entity
{
public:
	PracticeDummy();
	sf::CircleShape character;
};

#endif // PRACTICEDUMMY_H
