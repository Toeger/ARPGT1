#ifndef Rect_H
#define Rect_H

#include "physics_utility.h"

namespace Physical {
	//define an axis aligned rectangle with width and height starting at 0/0
	struct Rect {
		Rect(const Rect &other) = default;
		Rect &operator=(const Rect &) = default;
		Rect(float width, float height)
			: width(width)
			, height(height) {}
		float width;
		float height;
	};
}

#endif // Rect_H
