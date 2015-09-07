#include "physicalwall.h"

PhysicalWall::PhysicalWall(b2World &world, b2Vec2 center_position, b2Vec2 size)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(center_position.x, center_position.y);
    body = world.CreateBody(&bodyDef);
    b2PolygonShape box;
    box.SetAsBox(size.x / 2, size.y / 2);
    body->CreateFixture(&box, 0.0f);
}

PhysicalWall::~PhysicalWall()
{
    body->GetWorld()->DestroyBody(body);
}
