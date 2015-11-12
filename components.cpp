#include "components.h"
#include "entity.h"
#include <SFML/Graphics.hpp>
#include "physical.h"

void Components::add_CircleShape(Entity &entity, const float radius, const sf::Color color)
{
	sf::CircleShape cs(radius * 100);
	cs.setOrigin(radius * 100, radius * 100);
	cs.setFillColor(std::move(color));
	entity.add(std::move(cs));
}

void Components::add_PhysicalCircle(Entity &entity, float radius, std::pair<float, float> position)
{
	auto pc = Physical::create_physical_circle(radius, position);
	assert(pc);
	entity.add(std::move(pc));
}

void Components::add_PhysicalCircleShape(Entity &entity, const float radius, const std::pair<float, float> position, const sf::Color color)
{
	add_CircleShape(entity, radius, color);
	add_PhysicalCircle(entity, radius, position);
}

