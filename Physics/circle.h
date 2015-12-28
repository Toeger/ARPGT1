#ifndef CIRCLE_H
#define CIRCLE_H

#include "physics_utility.h"

namespace Physical {
	struct Circle
	{
		Circle(const Circle &other) = default;
		Circle &operator =(const Circle &) = default;
		Circle(const Transformator &t, float radius) :
			transformator(t),
			radius(radius)
		{}
		Transformator transformator;
		float radius;
	};

}
#endif // CIRCLE_H
