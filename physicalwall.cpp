#include "physicalwall.h"
#include "drawlist.h"

PhysicalWall::PhysicalWall(b2World &world, b2Vec2 center_position, b2Vec2 size,
                           bool visible, sf::Color color)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(center_position.x, center_position.y);
    body = world.CreateBody(&bodyDef);
    b2PolygonShape box;
    box.SetAsBox(size.x / 2, size.y / 2);
    body->CreateFixture(&box, 0.0f);
    if (visible){
        debugGraphics.setFillColor(color);
        debugGraphics.setPosition(center_position.x - size.x / 2, center_position.y - size.y / 2);
        debugGraphics.setSize({size.x, size.y});
        Drawlist::add(debugGraphics);
    }
}

PhysicalWall::~PhysicalWall()
{
    body->GetWorld()->DestroyBody(body);
}
