#ifndef AABB_H
#define AABB_H

#include "physics_utility.h"
#include "circle.h"

namespace Physical{
	struct AABB{
		AABB() :
		left(0),
		right(0),
		bottom(0),
		top(0)
		{}
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
			if (is_clear()){
				return *this = other;
			}
			left = std::min(left, other.left);
			right = std::max(right, other.right);
			bottom = std::min(bottom, other.bottom);
			top = std::max(top, other.top);
			return *this;
		}
		void clear(){
			left = right = bottom = top = 0;
		}
		bool is_clear() const{
			return left == 0 && right == 0 && bottom == 0 && top == 0;
		}
	};
	inline bool collides(const AABB &b1, const AABB &b2){
		return b1.left < b2.right && b1.right > b2.left && b1.bottom < b2.top && b1.top > b2.bottom;
	}
}

#endif // AABB_H
