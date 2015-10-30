#ifndef COMPONENTS
#define COMPONENTS

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "zombieai.h"

namespace Components{
using Physical_shape = b2Body *;
using sf::CircleShape;
using ZombieAi = ZombieAI::ZombieAI_component;
}

#endif // COMPONENTS
