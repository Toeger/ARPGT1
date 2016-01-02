#ifndef COLLISION_H
#define COLLISION_H

#include "shapes.h"

namespace Physical {
	namespace Helper{
		inline float square(float n){
			return n * n;
		}

		//check if a point x/y collides with an axis aligned rectangle with coordinates defined by left, right, bottom, top
		inline bool collides(float x, float y, float left, float right, float bottom, float top){
			return x > left && x < right && y > bottom && y < top;
		}
		//check if point x/y collides with circle with center at cx/cy and radius cr
		inline bool collides(float x, float y, float cx, float cy, float cr){
			return square(x - cx) + square(y - cy) < square(cr);
		}
	}

	inline bool collides(const Physical::Circle &c1, const Transformator &t1, const Physical::Circle &c2, const Transformator &t2){
		auto distance = t1.vector - t2.vector;
		using Helper::square;
		return square(distance.x) + square(distance.y) < square(c1.radius + c2.radius);
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
		Transformator t = t2;
		t.vector = -t.vector;
		-t.direction;
		t.direction = -t.direction - Direction{l.vector.x, l.vector.y};
		t += t1;
		using Helper::collides;
		return collides(t.vector.x, t.vector.y, 0, t.vector.length(), -c.radius, c.radius) || collides(0, 0, t.vector.x, t.vector.y, c.radius) || collides(t.vector.length(), 0, t.vector.x, t.vector.y, c.radius);
	}
	inline bool collides(const Physical::Line &l, const Transformator &t1, const Physical::Circle &c, const Transformator &t2){
		return collides(c, t2, l, t1);
	}
}

#endif // COLLISION_H
