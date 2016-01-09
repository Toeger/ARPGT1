#ifndef COLLISION_H
#define COLLISION_H

#include "shapes.h"

namespace Physical {
	namespace Helper{
		inline float square(float n){
			return n * n;
		}

		struct Point{
			float x, y;
		};

		struct Rect{
			float left, right, bottom, top;
			bool valid() const{
				return left <= right && bottom <= top;

			}
		};

		//check if a point x/y collides with an axis aligned rectangle with coordinates defined by left, right, bottom, top
		inline bool collides(Point p, Rect rect){
			assert(rect.valid());
			return p.x > rect.left && p.x < rect.right && p.y > rect.bottom && p.y < rect.top;
		}
		//check if point p collides with circle with center at cx/cy and radius cr
		inline bool collides(Point p, Point circle_center, float circle_radius){
			return square(p.x - circle_center.x) + square(p.y - circle_center.y) < square(circle_radius);
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
		//Idea: create a transformator that transforms l to be at points (0/0)-(length/0), then apply that transformator to c and do very simple collision detection
		//undo t2 transformation to have l's start point at (0/0)
		Transformator t = -t2;
		//rotate by inverse of l.vector direction so l.vector.y will be 0 and l.vector.x will be positive

		t += t1;
		t = Transformator(-Direction(l.vector.x, l.vector.y)) + t;

		using Helper::collides;
		bool collides_with_rect = collides({t.vector.x, t.vector.y}, {0, l.vector.length(), -c.radius, c.radius});
		bool collides_with_00 = collides({0, 0}, {t.vector.x, t.vector.y}, c.radius);
		bool collides_with_vec = collides({l.vector.length(), 0}, {t.vector.x, t.vector.y}, c.radius);
		return  collides_with_rect || collides_with_00 || collides_with_vec;
	}
	inline bool collides(const Physical::Line &l, const Transformator &t1, const Physical::Circle &c, const Transformator &t2){
		return collides(c, t2, l, t1);
	}
}

#endif // COLLISION_H
