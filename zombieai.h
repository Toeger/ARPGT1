#ifndef ZOMBIEAI_H
#define ZOMBIEAI_H

#include "entity.h"

namespace ZombieAI {
struct ZombieAI_component{};

class ZombieAI_system
{
public:
    static void update();
};
}

#endif // ZOMBIEAI_H
