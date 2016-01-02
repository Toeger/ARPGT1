#include <cassert>

#include "Physics/collision.h"

static void test_circle_circle_collisions(){
	Physical::Transformator t1({0, 0}, {}), t2({0, 0}, {});
	Physical::Circle c1(10), c2(20);
	assert(Physical::collides(c1, t1, c2, t2));
	t1.vector.x = 25;
	assert(Physical::collides(c1, t1, c2, t2));
	t1.vector.x = -25;
	assert(Physical::collides(c1, t1, c2, t2));
	t1.vector.x = 30;
	assert(!Physical::collides(c1, t1, c2, t2));
	t1.vector.x = -30;
	assert(!Physical::collides(c1, t1, c2, t2));
	t1.vector.x = 0;
	t1.vector.y = 25;
	assert(Physical::collides(c1, t1, c2, t2));
	t1.vector.y = -25;
	assert(Physical::collides(c1, t1, c2, t2));
	t1.vector.y = 30;
	assert(!Physical::collides(c1, t1, c2, t2));
	t1.vector.y = -30;
	assert(!Physical::collides(c1, t1, c2, t2));
	t1.vector.y = 0;

	t2.vector.x = 25;
	assert(Physical::collides(c1, t1, c2, t2));
	t2.vector.x = -25;
	assert(Physical::collides(c1, t1, c2, t2));
	t2.vector.x = 30;
	assert(!Physical::collides(c1, t1, c2, t2));
	t2.vector.x = -30;
	assert(!Physical::collides(c1, t1, c2, t2));
	t2.vector.x = 0;
	t2.vector.y = 25;
	assert(Physical::collides(c1, t1, c2, t2));
	t2.vector.y = -25;
	assert(Physical::collides(c1, t1, c2, t2));
	t2.vector.y = 30;
	assert(!Physical::collides(c1, t1, c2, t2));
	t2.vector.y = -30;
	assert(!Physical::collides(c1, t1, c2, t2));
	t2.vector.y = 0;
}

static void test_line_line_collisions(){
	Physical::Line l1(0, 0), l2(0, 0);
	Physical::Transformator t1({}, {}), t2({}, {});
	assert(Physical::collides(l1, t1, l2, t2));
	l1.vector.x = 10;
	l2.vector.x = 10;
	assert(Physical::collides(l1, t1, l2, t2));
	t1.vector.y = 1;
	//assert(!Physical::collides(l1, t1, l2, t2));
}

static void test_line_circle_collision(){
	Physical::Line l(5, 0);
	Physical::Circle c(1);
	Physical::Transformator t1({}, {}), t2({}, {});
	assert(Physical::collides(c, t1, l, t2));
	t1.vector.x = 2;
	t1.vector.y = 2;
	assert(!Physical::collides(c, t1, l, t2));
	t1.vector.y = 2;
	assert(!Physical::collides(c, t1, l, t2));
	t1.vector.x = t1.vector.y = -1;
	assert(!Physical::collides(c, t1, l, t2));
	t1.vector.y = 1;
	assert(!Physical::collides(c, t1, l, t2));
	t1.vector.x = 6;
	assert(!Physical::collides(c, t1, l, t2));
	t1.vector.y = -1;
	assert(!Physical::collides(c, t1, l, t2));

	c.radius = 2;
	t1.vector.x = t1.vector.y = -1;
	assert(Physical::collides(c, t1, l, t2));
	t1.vector.y = 1;
	assert(Physical::collides(c, t1, l, t2));
	t1.vector.x = 6;
	assert(Physical::collides(c, t1, l, t2));
	t1.vector.y = -1;
	assert(Physical::collides(c, t1, l, t2));

	c.radius = 3;
	t1.vector.x = 2;
	t1.vector.y = 2;
	assert(Physical::collides(c, t1, l, t2));
	t1.vector.y = 2;
	assert(Physical::collides(c, t1, l, t2));

}

void test_collisions(){
	test_circle_circle_collisions();
	test_line_line_collisions();
	test_line_circle_collision();
}
