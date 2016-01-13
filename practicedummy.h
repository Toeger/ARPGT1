#ifndef PRACTICEDUMMY_H
#define PRACTICEDUMMY_H

#include <SFML/Graphics.hpp>
#include "ECS/entity.h"

class PracticeDummy : public ECS::Entity
{
public:
	PracticeDummy();
	sf::CircleShape character;
};

#endif // PRACTICEDUMMY_H
