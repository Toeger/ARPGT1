#include "zombieai.h"
#include "entity.h"
#include "components.h"
#include "player.h"

void ZombieAI::ZombieAI_system::update()
{
    const auto &playerpos = (*Player::player.get<Components::Physical_shape>())->GetPosition();
    for (auto sit = System::range<Components::Physical_shape, Components::ZombieAi>(); sit; sit.advance()){
        auto &ps = *sit.get<Components::Physical_shape>();
        const auto &pos = ps.GetPosition();
        b2Vec2 vel = playerpos - pos;
        vel.Normalize();
        vel *= 3;
        ps.SetLinearVelocity(vel);
    }
}
