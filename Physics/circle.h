#ifndef CIRCLE_H
#define CIRCLE_H

#include "physics_utility.h"

namespace Physical {
	struct Circle : Transformator
	{
		Circle(const Circle &other) = default;
		Circle(Circle &&other) = default;
		Circle(const Transformator &t, float radius) :
			Transformator(t),
			radius(radius)
		{}
		float radius;
	};

}
#endif // CIRCLE_H
