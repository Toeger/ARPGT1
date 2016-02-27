#ifndef LINE_H
#define LINE_H

#include "physics_utility.h"

#include <cmath>

namespace Physical{
	//Defines a physical line with startpoint 0/0 and endpoint defined by vector
	struct Line{
		Line(float x, float y) :
			vector(x, y)
		{}
		Line(const Vector &vector) :
			vector(vector)
		{}
		Line &operator =(const Line &) = default;
		Line(const Line &) = default;
		Vector vector;
	};
}

#endif // LINE_H
