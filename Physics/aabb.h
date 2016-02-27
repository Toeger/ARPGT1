#ifndef AABB_H
#define AABB_H

#include <limits>
#include <iostream>
#include <algorithm>

#include "physics_utility.h"
#include "shapes.h"

namespace Physical{
	struct AABB{
		constexpr AABB()
		{
			clear();
		}
		constexpr AABB(const Vector &bottom_left, float width, float height) :
			left(bottom_left.x),
			right(bottom_left.x + width),
			bottom(bottom_left.y),
			top(bottom_left.y + height)
		{}
		constexpr AABB(float left, float right, float bottom, float top) :
			left(left),
			right(right),
			bottom(bottom),
			top(top)
		{}
		constexpr AABB(const Physical::Circle &c, const Transformator &t) :
		left(t.vector.x - c.radius),
		right(t.vector.x + c.radius),
		bottom(t.vector.y - c.radius),
		top(t.vector.y + c.radius)
		{}
		constexpr AABB(const Physical::Line &c, const Transformator &t)
		{
			const auto &p1 = t.vector;
			const auto &p2 = t + c.vector;
			auto left_right = std::minmax(p1.x, p2.vector.x);
			left = left_right.first;
			right = left_right.second;
			bottom = std::min(p1.y, p2.vector.y);
			top = std::max(p1.y, p2.vector.y);
		}
		constexpr AABB(const AABB &other) = default;
		~AABB() = default;
		constexpr AABB &operator = (const AABB &other) = default;
		constexpr AABB &combine(const AABB &other){
			left = std::min(left, other.left);
			right = std::max(right, other.right);
			bottom = std::min(bottom, other.bottom);
			top = std::max(top, other.top);
			return *this;
		}
		constexpr void clear(){
			left = bottom = std::numeric_limits<float>::max();
			right = top = -std::numeric_limits<float>::max(); //std::numeric_limits<float>::min() returns a positive number
		}

		//Data
		float left = std::numeric_limits<float>::max();
		float right = -std::numeric_limits<float>::max();
		float bottom = std::numeric_limits<float>::max();
		float top = -std::numeric_limits<float>::max();
	};
	inline std::ostream &operator <<(std::ostream &os, const AABB &aabb){
		return os << '[' << aabb.left << ',' << aabb.top << ',' << aabb.right << ',' << aabb.bottom << ']';
	}
	inline bool collides(const AABB &b1, const AABB &b2){
		return b1.left < b2.right && b1.right > b2.left && b1.bottom < b2.top && b1.top > b2.bottom;
	}
}

#endif // AABB_H
