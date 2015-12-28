#ifndef CIRCLE_H
#define CIRCLE_H

#include "physics_utility.h"

namespace Physical {
	struct Circle
	{
		Circle(const Circle &other) = default;
		Circle &operator =(const Circle &) = default;
		Circle(float radius) :
			radius(radius)
		{}
		float radius;
	};

}
#endif // CIRCLE_H
