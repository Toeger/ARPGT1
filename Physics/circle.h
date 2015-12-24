#ifndef CIRCLE_H
#define CIRCLE_H

#include "physics_utility.h"

namespace Physical {
	struct Circle : Transformator
	{
		Circle(const Circle &other) = default;
		Circle &operator =(const Circle &) = default;
		Circle(const Transformator &t, float radius) :
			Transformator(t),
			radius(radius)
		{}
		Circle(const Vector &pos, float radius) :
			Transformator(Transformator::get_translation_matrix(pos)),
			radius(radius)
		{}
		float radius;
	};

}
#endif // CIRCLE_H
