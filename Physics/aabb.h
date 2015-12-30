#ifndef AABB_H
#define AABB_H

#include "physics_utility.h"

namespace Physical{
	struct AABB{
		AABB(const Vector &bottom_left, float width, float height) :
			left(bottom_left.x),
			right(bottom_left.x + width),
			bottom(bottom_left.y),
			top(bottom_left.y + height)
		{}
		AABB(float left, float right, float bottom, float top) :
			left(left),
			right(right),
			bottom(bottom),
			top(top)
		{}
		AABB(const AABB &other) = default;
		~AABB() = default;
		AABB &operator = (const AABB &other) = default;
		float left, right, bottom, top;
	};
	inline bool collides(const AABB &b1, const AABB &b2){
		return b1.left < b2.right && b1.right > b2.left && b1.bottom < b2.top && b1.top > b2.bottom;
	}
}

#endif // AABB_H
