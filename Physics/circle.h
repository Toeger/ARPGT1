#ifndef CIRCLE_H
#define CIRCLE_H

#include "physics_utility.h"

namespace Physical {
	struct Circle
	{
		Circle(const Circle &other) :
			position(other.position),
			radius(other.radius)
		{
		}

		Circle(Physical::Vector position, float radius) :
			position(position),
			radius(radius)
		{}
		Physical::Vector position;
		float radius;
	};

}
#endif // CIRCLE_H
