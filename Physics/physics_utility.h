#ifndef UTILITY_H
#define UTILITY_H

namespace Physical {
	//A Vector defines a position through its x and y coordinate
	//has nothing to do with std::vector
	struct Vector{
		Vector(float x = 0, float y = 0) :
			x(x),
			y(y)
		{
		}
		float x, y;
	};

	//A Direction defines the direction an object is oriented towards
	//A direction of (x=1, y=0) means the object is rotated by 0 degrees
	//A direction of (x=1, y=1) means the object is rotated by 45 degrees counter clockwise
	struct Direction : Vector{
		//TODO: implement stuff
	};
}

#endif // UTILITY_H
