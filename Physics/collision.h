#ifndef COLLISION_H
#define COLLISION_H

#include "shapes.h"

namespace Physical {
	inline bool collides(const Physical::Circle &c1, const Transformator &t1, const Physical::Circle &c2, const Transformator &t2){
		auto distance = t1.vector - t2.vector;
		return distance.x * distance.x + distance.y * distance.y < (c1.radius + c2.radius) * (c1.radius + c2.radius);
	}
	inline bool collides(const Physical::Line &l1, const Transformator &t1, const Physical::Line &l2, const Transformator &t2){
		//TODO
		(void)l1;
		(void)l2;
		(void)t1;
		(void)t2;
		return true;
	}
	inline bool collides(const Physical::Circle &c, const Transformator &t1, const Physical::Line &l, const Transformator &t2){
		//TODO
		(void)c;
		(void)l;
		(void)t1;
		(void)t2;
		return true;
	}
	inline bool collides(const Physical::Line &l, const Transformator &t1, const Physical::Circle &c, const Transformator &t2){
		return collides(c, t2, l, t1);
	}
}

#endif // COLLISION_H
