#include "physics/collision.h"
#include "ecs/entity.h"
#include "gameplay/map.h"
#include "physics/body.h"
#include "tests/tests_utility.h"

#include <cassert>

static void test_helper_collides_point_circle() {
	float testnumbers[] = {0, .1f, 1, 10, 33, 444, -.1f, -1, -10, -33, -444};
	for (float dx : testnumbers) {
		for (float dy : testnumbers) {
			for (float radius : {1.f, 0.1f, 100.f}) {
				assert(Physical::Helper::collides({dx, dy}, {dx, dy}, radius));
				assert(Physical::Helper::collides({dx + radius * 0.8f, dy}, {dx, dy}, radius));
				assert(Physical::Helper::collides({dx - radius * 0.8f, dy}, {dx, dy}, radius));
				assert(Physical::Helper::collides({dx, dy + radius * 0.8f}, {dx, dy}, radius));
				assert(Physical::Helper::collides({dx, dy + radius * 0.8f}, {dx, dy}, radius));
				for (float sign1 : {1, -1}) {
					for (float sign2 : {1, -1}) {
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

static void test_helper_collides_point_rect() {
	float testnumbers[] = {0, .1f, 1, 10, 33, 444, -.1f, -1, -10, -33, -444};
	for (float x : testnumbers) {
		for (float y : testnumbers) {
			for (float d : {0.1f, 1.f, 10.f, 33.f, 444.f}) {
				assert(Physical::Helper::collides({x, y}, {x - d, x + d, y - d, y + d}));
				assert(!Physical::Helper::collides({x, y}, {x + d, x + 2 * d, y - d, y + d}));
				assert(!Physical::Helper::collides({x, y}, {x - 2 * d, x - d, y - d, y + d}));
				assert(!Physical::Helper::collides({x, y}, {x - d, x + d, y + d, y + 2 * d}));
				assert(!Physical::Helper::collides({x, y}, {x - d, x + d, y - 2 * d, y - d}));
			}
		}
	}
}

static void test_circle_circle_collisions() {
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

static void test_line_line_collisions() {
	Physical::Line l1(0, 0), l2(0, 0);
	Physical::Transformator t1({}, {}), t2({}, {});
	assert(Physical::collides(l1, t1, l2, t2));
	l1.vector.x = 10;
	l2.vector.x = 10;
	assert(Physical::collides(l1, t1, l2, t2));
	t1.vector.y = 1;
	//assert(!Physical::collides(l1, t1, l2, t2));
}

static void test_line_circle_collision() {
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

	float testnumbers[] = {0, .1f, 1, 10, 33, 444, -.1f, -1, -10, -33, -444};
	for (auto x : testnumbers) {
		for (auto y : testnumbers) {
			for (auto dx : testnumbers) {
				for (auto dy : testnumbers) {
					Physical::Transformator t({x, y}, {dx, dy});
					for (auto &cleared : {t + -t, -t + t}) {
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
	for (auto &t : transformators) {
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

static void run_towards_each_other_and_get_stuck_bug_test() {
	ECS::Entity e1;
	ECS::Entity e2;
	e1.add(Physical::DynamicBody<Physical::Circle>(100));
	e2.add(Physical::DynamicBody<Physical::Circle>(100, 1000));
	auto &b1 = *e1.get<Physical::DynamicBody<Physical::Circle>>();
	auto &b2 = *e2.get<Physical::DynamicBody<Physical::Circle>>();
	b1 += 500;
	b2 += -500;
	assert_equal(b1.get_next_transformator().vector.x, 500.f);
	assert_equal(b2.get_next_transformator().vector.x, 1000.f);
}

static void test_circle_rect() {
	const auto r = 100.f;
	const auto w = 200.f;
	const auto h = 300.f;
	const Physical::Circle cicle(r);
	const Physical::Rect rect(w, h);
	const float testnumbers[] = {0, .1f, 1, 10, 33, 444, -.1f, -1, -10, -33, -444};
	for (auto x : testnumbers) {
		for (auto y : testnumbers) {
			for (auto dx : testnumbers) {
				for (auto dy : testnumbers) {
					Physical::Transformator t({x, y}, {dx, dy});
					//positive tests
					auto factor = 0.9f;
					Physical::Transformator t1;
					Physical::Transformator t2;
					assert(Physical::collides(cicle, t + t1, rect, t + t2));
					t1.vector.x = w + r * factor;
					assert(Physical::collides(cicle, t + t1, rect, t + t2));
					t1.vector.y = h + r * factor;
					assert(Physical::collides(cicle, t + t1, rect, t + t2));
					t1.vector.x = -factor * r;
					assert(Physical::collides(cicle, t + t1, rect, t + t2));
					t1.vector.y = -factor * r;
					assert(Physical::collides(cicle, t + t1, rect, t + t2));
					//negative tests
					factor = 1.1f;
					t1.vector.x = w + r * factor;
					t1.vector.y = 0;
					assert(!Physical::collides(cicle, t + t1, rect, t + t2));
					t1.vector.y = h + r * factor;
					t1.vector.x = 0;
					assert(!Physical::collides(cicle, t + t1, rect, t + t2));
					t1.vector.x = -factor * r;
					t1.vector.y = 0;
					assert(!Physical::collides(cicle, t + t1, rect, t + t2));
					t1.vector.y = -factor * r;
					t1.vector.x = 0;
					assert(!Physical::collides(cicle, t + t1, rect, t + t2));
				}
			}
		}
	}
}

static void test_collision_with_map() {}

void test_collisions() {
	test_helper_collides_point_circle();
	test_helper_collides_point_rect();
	test_circle_circle_collisions();
	test_line_line_collisions();
	test_line_circle_collision();
	test_circle_rect();
	ECS::Entity map;
	Map::current_map = &map.emplace<Map>(1, 1);
	run_towards_each_other_and_get_stuck_bug_test();
	test_collision_with_map();
}
