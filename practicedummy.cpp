#include "practicedummy.h"
#include "conversions.h"
#include "hittable.h"
#include <memory>
#include "enemylist.h"

PracticeDummy::PracticeDummy() :
    character(60)
{
    character.setFillColor({200, 50, 50});
}
