#ifndef COMMON_COMPONENTS_H
#define COMMON_COMPONENTS_H

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
}

#endif // COMMON_COMPONENTS_H
