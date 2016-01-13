#include <cassert>

#include "Physics/collision.h"
#include "Tests/tests_utility.h"
#include "entity.h"
#include "Physics/body.h"

static void test_helper_collides_point_circle(){
	float testnumbers[] = {0, .1, 1, 10, 33, 444, -.1, -1, -10, -33, -444};
	for (float dx : testnumbers){
		for (float dy : testnumbers){
			for (float radius : {1.f, 0.1f, 100.f}){
				assert(Physical::Helper::collides({dx, dy}, {dx, dy}, radius));
				assert(Physical::Helper::collides({dx + radius * 0.8f, dy}, {dx, dy}, radius));
				assert(Physical::Helper::collides({dx - radius * 0.8f, dy}, {dx, dy}, radius));
				assert(Physical::Helper::collides({dx, dy + radius * 0.8f}, {dx, dy}, radius));
				assert(Physical::Helper::collides({dx, dy + radius * 0.8f}, {dx, dy}, radius));
				for (float sign1 : {1, -1}){
					for (float sign2 : {1, -1}){
						assert(Physical::Helper::collides({dx + sign1 * radius * 0.7f, dy + sign2 * radius * 0.7f}, {dx, dy}, radius));
						assert(!Physical::Helper::collides({dx + sign1 * radius * 1.1f, dy}, {dx, dy}, radius));
						assert(!Physical::Helper::collides({dx, dy + sign1 * radius * 1.1f}, {dx, dy}, radius));
						assert(!Physical::Helper::collides({dx + sign1 * radius * 0.8f, dy + sign2 * radius * 0.8f}, {dx, dy}, radius));
					}
				}
			}
		}
	}
}

static void test_helper_collides_point_rect(){
	float testnumbers[] = {0, .1, 1, 10, 33, 444, -.1, -1, -10, -33, -444};
	for (float x : testnumbers){
		for (float y : testnumbers){
			for (float d : {0.1f, 1.f, 10.f, 33.f, 444.f}){
				assert(Physical::Helper::collides({x, y}, {x - d, x + d, y - d, y + d}));
				assert(!Physical::Helper::collides({x, y}, {x + d, x + 2 * d, y - d, y + d}));
				assert(!Physical::Helper::collides({x, y}, {x - 2 * d, x - d, y - d, y + d}));
				assert(!Physical::Helper::collides({x, y}, {x - d, x + d, y + d, y + 2 * d}));
				assert(!Physical::Helper::collides({x, y}, {x - d, x + d, y - 2 * d, y - d}));
			}
		}
	}
}

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
					for (auto &cleared : {t + -t, -t + t}){
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
		Physical::Transformator cleared = t + -t;
		assert_equal(cleared.vector.length(), 0);
		assert_equal(cleared.direction.get_x(), 1);
		assert_equal(cleared.direction.get_y(), 0);
		cleared = -t + t;
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

static void test_failcase(){ //bug reproduction test case
	ECS::Entity p;
	{
		Physical::Body b;
		b.attach(Physical::Circle(30), {100, 50}, {});
		b.attach(Physical::Circle(30), {-100, 50}, {});
		b.attach(Physical::Circle(100), {}, {});
		b.attach(Physical::Line(350, 0), {}, {0, 1});
		p.add(std::move(b));
	}
	ECS::Entity dots[4];
	{
		int counter = 0;
		Physical::Vector ps[] = {{1000, 1000}, {-1000, 1000}, {1000, -1000}, {-1000, -1000}};
		for (auto &p : ps){
			Physical::Body b;
			b.attach(Physical::Circle(10), p, {});
			 dots[counter++].add(std::move(b));
		}
	}
	auto &b = *p.get<Physical::Body>();
	b += Physical::Vector(-871.733, 956.152);
	assert_equal(b.get_next_transformator().vector.x, b.get_current_transformator().vector.x);
}

void test_collisions(){
	test_helper_collides_point_circle();
	test_helper_collides_point_rect();
	test_circle_circle_collisions();
	test_line_line_collisions();
	test_line_circle_collision();
	test_failcase();
}
