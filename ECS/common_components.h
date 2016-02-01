#ifndef COMMON_COMPONENTS_H
#define COMMON_COMPONENTS_H

#include "Graphics/animations.h"
#include <chrono>

namespace Common_components{
	struct Speed{
		float speed;
	};
	struct HP{
		HP(int hp)
			:hp(hp)
			,max_hp(hp)
		{}
		int hp;
		int max_hp;
	};
	struct Life_time{
		int life_time; //in logical frames left
	};
	struct Animation{
		Animations::Animation animation;
		float start_frame = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() % 1000 / 1000.f;
	};
}

#endif // COMMON_COMPONENTS_H
