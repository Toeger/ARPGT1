#ifndef PHYSICAL_H
#define PHYSICAL_H
#include <utility>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <memory>

namespace Physical
{
	void body_deleter(b2Body *body);
	std::unique_ptr<b2Body, void (*)(b2Body*)> create_physical_circle(float radius, std::pair<float, float> position);
	extern b2World *world;
}

#endif // PHYSICAL_H
