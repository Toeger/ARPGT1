#include <cassert>

#include "Physics/collision.h"
#include "Tests/tests_utility.h"

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

	l.vector.x = -2;
	l.vector.y = -6;
	c.radius = 1;
	t1.vector.x = 0;
	t1.vector.y = 0;

	float testnumbers[] = {0, .1, 1, 10, 33, 444, -.1, -1, -10, -33, -444};
	for (auto  x : testnumbers){
		for (auto y : testnumbers){
			for (auto dx : testnumbers){
				for (auto dy : testnumbers){
					Physical::Transformator t({x, y}, {dx, dy});
					for (auto &cleared : {t + t.right_inverse(), t.left_inverse() + t}){
						assert_equal(cleared.vector.x, 0);
						assert_equal(cleared.vector.y, 0);
						assert_equal(cleared.direction.get_x(), 1);
						assert_equal(cleared.direction.get_y(), 0);
					}
				}
			}
		}
	}
	Physical::Transformator transformators[] = {{{}, {}}, {{199, 323}, {23, 49}}, {{199, -323}, {-23, 49}}, {{-199, 323}, {23, -49}}, {{-199, 323}, {-23, 49}}};
	for (auto &t : transformators){
		Physical::Transformator cleared = t + t.right_inverse();
		assert_equal(cleared.vector.length(), 0);
		assert_equal(cleared.direction.get_x(), 1);
		assert_equal(cleared.direction.get_y(), 0);
		cleared = t.left_inverse() + t;
		assert_equal(cleared.vector.length(), 0);
		assert_equal(cleared.direction.get_x(), 1);
		assert_equal(cleared.direction.get_y(), 0);
		t2.vector.x = -2;
		t2.vector.y = -2;
		assert(!Physical::collides(l, t + t1, c, t + t2));
		t2.vector.x = 0;
		t2.vector.y = -4;
		assert(!Physical::collides(l, t + t1, c, t + t2));
		t2.vector.x = 1;
		t2.vector.y = 1;
		assert(!Physical::collides(l, t + t1, c, t + t2));
		t2.vector.x = -3;
		t2.vector.y = -7;
		assert(!Physical::collides(l, t + t1, c, t + t2));
		t2.vector.x = -1;
		t2.vector.y = -2;
		assert(Physical::collides(l, t + t1, c, t + t2));
		t2.vector.x = -1;
		t2.vector.y = -4;
		assert(Physical::collides(l, t + t1, c, t + t2));
	}

}

void test_collisions(){
	test_circle_circle_collisions();
	test_line_line_collisions();
	test_line_circle_collision();
}
