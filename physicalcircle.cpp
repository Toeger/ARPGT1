#include "physicalcircle.h"

PhysicalCircle::PhysicalCircle(b2World &world, b2Vec2 position, float radius, float friction)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    body = world.CreateBody(&bodyDef);
    b2CircleShape characterShape;
    characterShape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &characterShape;
    fixtureDef.density = 1.f;
    fixtureDef.friction = friction;
    body->CreateFixture(&fixtureDef);
    body->SetLinearDamping(.5f);
}

b2Vec2 PhysicalCircle::get_position() const
{
    return body->GetPosition();
}

void PhysicalCircle::apply_position(sf::CircleShape &circle) const
{
    auto box2dPos = body->GetPosition();
    sf::Vector2f pos = {box2dPos.x - circle.getRadius(), box2dPos.y - circle.getRadius()};
    circle.setPosition(pos);
}

void PhysicalCircle::set_velocity(b2Vec2 velocity)
{
    body->SetLinearVelocity(velocity);
    body->SetAwake(true);
}

b2World *PhysicalCircle::get_world()
{
    return body->GetWorld();
}

PhysicalCircle::~PhysicalCircle()
{
    body->GetWorld()->DestroyBody(body);
}
