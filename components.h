#ifndef COMPONENTS
#define COMPONENTS

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "zombieai.h"
#include <physical.h>
#include "utility.h"
#include <utility>
#include <SFML/Graphics/Color.hpp>
struct Entity;

namespace Components{
	void add_CircleShape(Entity &entity, const float radius, const sf::Color color);
	using Circle_shape = sf::CircleShape;
	void add_PhysicalCircle(Entity &entity, const float radius, const std::pair<float, float> position);
	using Physical_circle = Utility::return_type_t<decltype(Physical::create_physical_circle)>;
	void add_PhysicalCircleShape(Entity &entity, const float radius, const std::pair<float, float> position, const sf::Color color);
	using ZombieAi = ZombieAI::ZombieAI_component;
	void add_circle_sensor(Entity &entity, const float radius, const std::pair<float, float> offset = {0,0});
}

#endif // COMPONENTS
