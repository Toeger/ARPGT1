#ifndef CIRCLE_H
#define CIRCLE_H

#include "physics_utility.h"
#include "aabb.h"

namespace Physical {
	struct Circle
	{
		Circle(const Circle &other) = default;
		Circle &operator =(const Circle &) = default;
		Circle(float radius) :
			radius(radius)
		{}
		AABB get_AABB() const{
			return {-radius, -radius, 2 * radius, 2 * radius};
		}
		float radius;
	};

}
#endif // CIRCLE_H
