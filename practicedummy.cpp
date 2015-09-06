#include "practicedummy.h"
#include "drawlist.h"
#include "conversions.h"
#include "logicalobject.h"

PracticeDummy::PracticeDummy(b2World &world) :
character(40)
{
    character.setFillColor({200, 50, 50});
    Drawlist::add(character);
    LogicalObject::add(*this);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(400, 300);
    body = world.CreateBody(&bodyDef);
    b2CircleShape characterShape;
    characterShape.m_radius = 50;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &characterShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.f;
    body->CreateFixture(&fixtureDef);
}

void PracticeDummy::logicalUpdate()
{
    character.setPosition(
                Conversions::boxPosToSfCirclePos(
                    body->GetPosition(),
                    character.getRadius()));
}

