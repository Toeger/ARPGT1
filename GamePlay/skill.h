#ifndef SKILL_H
#define SKILL_H

#include "Graphics/common_graphics_components.h"
#include "ECS/entity.h"

#include <string>
#include <bitset>

namespace Skills
{
	void load_skills(const std::string &file_path);
	enum class Type{
		projectile,
		aura,
		instant,
	};
	enum Collisions{
		enemies,
		allies,
		self,
		map,
		size
	};

	struct Skill{
		void activate();
		void cancel();
		std::string name;
		float channeltime = 0.f;
		float executiontime = 0.f;
		float cooldown = 0.f;
		float size = 0.f;
		int time = 0;
		Common_components::Animated_model animation;
		ECS::Entity_handle owner;
		Skill *effect = nullptr;
		Type type; //tag that defines the content of data
		union{
			struct Projectile{
				ECS::Entity_handle target;
				float turnspeed; //in degrees per tick
				float speed = 0.f;
				std::bitset<Collisions::size> collisions;
			};
			struct Aura{
				std::bitset<Collisions::size> collisions;
			};
			struct Instant{
				ECS::Entity_handle target;
			};
		} data;
	};
}

#endif // SKILL_H
