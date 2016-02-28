#ifndef AARECT_H
#define AARECT_H

#include "physics_utility.h"

namespace Physical {
	//define an axis aligned rectangle with width and height starting at 0/0
	struct AARect
	{
		AARect(const AARect &other) = default;
		AARect &operator =(const AARect &) = default;
		AARect(float width, float height)
			: width(width)
			, height(height)
		{}
		float width;
		float height;
	};

}

#endif // AARECT_H
