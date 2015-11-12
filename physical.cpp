#include "physical.h"
#include "entity.h"
#include <Box2D/Box2D.h>

b2World *Physical::world;

void Physical::body_deleter(b2Body *body)
{
	assert(world);
	world->DestroyBody(body);
}

std::unique_ptr<b2Body, void (*)(b2Body*)> Physical::create_physical_circle(float radius, std::pair<float, float> position)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.first, position.second);

	std::unique_ptr<b2Body, void (*)(b2Body*)> body{Physical::world->CreateBody(&bodyDef), Physical::body_deleter};
	b2CircleShape characterShape;
	characterShape.m_radius = radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &characterShape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0;
	body->CreateFixture(&fixtureDef);
	body->SetLinearDamping(5.0f);

	return body;
}
