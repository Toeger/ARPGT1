#include "sensor.h"



std::vector<b2Body *> Sensor::get_collisions(b2Body *body)
{
    //body->GetWorld()->Step(0, 1, 1);
    std::vector<b2Body *> retval;
    for (auto worldcontacts = body->GetWorld()->GetContactList();
         worldcontacts;
         worldcontacts = worldcontacts->GetNext()){
        auto b1 = worldcontacts->GetFixtureA()->GetBody();
        auto b2 = worldcontacts->GetFixtureB()->GetBody();
        if (b1 == body){
            retval.push_back(b2);
        }
        else if (b2 == body){
            retval.push_back(b1);
        }
    }

    //should work but doesn't

    for (auto edge = body->GetContactList(); edge; edge = edge->next){
        retval.push_back(edge->other);
    }

    return retval;
}
