#include "sensor.h"

std::vector<b2Body *> Sensor::get_collisions(b2Body *body)
{
    std::vector<b2Body *> retval;
    for (auto edge = body->GetContactList(); edge; edge = edge->next){
        retval.push_back(edge->other);
    }

    return retval;
}
