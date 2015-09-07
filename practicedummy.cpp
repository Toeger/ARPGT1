#include "practicedummy.h"
#include "drawlist.h"
#include "conversions.h"
#include "logicalobject.h"

PracticeDummy::PracticeDummy(b2World &world) :
    character(40),
    pc(world, {600, 300}, 40)
{
    character.setFillColor({200, 50, 50});
    Drawlist::add(character);
    LogicalObject::add(*this);
}

void PracticeDummy::logicalUpdate()
{
    pc.apply_position(character);

}
