#ifndef AABB_H
#define AABB_H

#include <limits>
#include <iostream>

#include "physics_utility.h"
#include "circle.h"

namespace Physical{
	struct AABB{
		AABB()
		{
			clear();
		}
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
		AABB(const Physical::Circle &c, const Transformator &t) :
		left(t.vector.x - c.radius),
		right(t.vector.x + c.radius),
		bottom(t.vector.y - c.radius),
		top(t.vector.y + c.radius)
		{}
		AABB(const AABB &other) = default;
		~AABB() = default;
		AABB &operator = (const AABB &other) = default;
		float left, right, bottom, top;
		AABB &combine(const AABB &other){
			left = std::min(left, other.left);
			right = std::max(right, other.right);
			bottom = std::min(bottom, other.bottom);
			top = std::max(top, other.top);
			return *this;
		}
		void clear(){
			left = bottom = std::numeric_limits<float>::max();
			right = top = -std::numeric_limits<float>::max(); //std::numeric_limits<float>::min() returns a positive number
		}
	};
	inline std::ostream &operator <<(std::ostream &os, const AABB &aabb){
		return os << '[' << aabb.left << ',' << aabb.top << ',' << aabb.right << ',' << aabb.bottom << ']';
	}
	inline bool collides(const AABB &b1, const AABB &b2){
		return b1.left < b2.right && b1.right > b2.left && b1.bottom < b2.top && b1.top > b2.bottom;
	}
}

#endif // AABB_H