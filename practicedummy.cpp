#include "practicedummy.h"
#include "drawlist.h"
#include "conversions.h"
#include "logicalobject.h"
#include "hittable.h"
#include <memory>

PracticeDummy::PracticeDummy(b2World &world) :
    character(60),
    pc(world, {600, 300}, 60)
{
    character.setFillColor({200, 50, 50});
    Drawlist::add(character);
    LogicalObject::add(*this);
    pc.body->SetUserData(std::make_unique<Hittable>(this).release());
}

void PracticeDummy::logicalUpdate()
{
    pc.apply_position(character);

}

void PracticeDummy::take_hit(Weapon &weapon)
{
    auto source_position = weapon.fixture->GetBody()->GetPosition();
    b2Vec2 my_position = pc.body->GetPosition();
    b2Vec2 source_direction = source_position - my_position;
    source_direction.Normalize();
    b2Vec2 knock_back_direction = -source_direction;
    const auto knock_back_strength = 10;
    knock_back_direction.x *= knock_back_strength;
    knock_back_direction.y *= knock_back_strength;
    pc.body->SetLinearVelocity(knock_back_direction);
}
