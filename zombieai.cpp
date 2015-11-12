#include "zombieai.h"
#include "entity.h"
#include "components.h"
#include "player.h"

void ZombieAI::ZombieAI_system::update()
{
	const auto &playerpos = (*Player::player.get<Components::PhysicalCircle>())->GetPosition();
	for (auto sit = System::range<Components::PhysicalCircle, Components::ZombieAi>(); sit; sit.advance()){
		auto &ps = *sit.get<Components::PhysicalCircle>();
		const auto &pos = ps.GetPosition();
		b2Vec2 vel = playerpos - pos;
		vel.Normalize();
		vel *= 3;
		ps.SetLinearVelocity(vel);
	}
}
