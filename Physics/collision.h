#ifndef COLLISION_H
#define COLLISION_H

#include "circle.h"

namespace Physical {
	inline bool collides(const Physical::Circle &c1, const Transformator &t1, const Physical::Circle &c2, const Transformator &t2){
		auto distance = t1.vector - t2.vector;
		return distance.x * distance.x + distance.y * distance.y < (c1.radius + c2.radius) * (c1.radius + c2.radius);
	}
}

#endif // COLLISION_H
