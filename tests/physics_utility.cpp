#include "physics/physics_utility.h"
#include "tests/tests_utility.h"

using namespace Physical;

static void test_vector() {
	Vector v;
	assert_equal(v.x, 0);
	assert_equal(v.y, 0);
	assert_equal(v.length(), 0);
	v.x = 3;
	v.y = 4;
	assert_equal(v.length(), 5);
}

static void test_direction() {
	Direction d;
	assert_equal(d.to_degrees(), 0);
	d = Direction::from_degrees(30);
	assert_equal((-d).to_degrees(), -30);
	assert_equal((d + d).to_degrees(), 60);
	assert_equal((d + d - d).to_degrees(), 30);
}

static void test_transformator() {
	Transformator t;
	assert_equal(t.direction.get_x(), 1);
	assert_equal(t.direction.get_y(), 0);
	assert_equal(t.vector.x, 0);
	assert_equal(t.vector.y, 0);

	t.vector.x = 22;
	t.vector.y = 33;
	Transformator t2 = t;
	assert_equal(t2.vector.x, 22);
	assert_equal(t2.vector.y, 33);
	t2.vector.x = 55;
	t2.vector.y = 77;
	auto t3 = t + t2;
	assert_equal(t3.vector.x, t.vector.x + t2.vector.x);
	assert_equal(t3.vector.y, t.vector.y + t2.vector.y);
	t2.direction = Direction::from_degrees(90);
	t.direction = Direction::from_degrees(-30);
	t3 = t2 + t;
	assert_equal(t3.vector.x, 22);
	assert_equal(t3.vector.y, 99);
	assert_equal(t3.direction.to_degrees(), 60);

	auto t4 = -t3;
	assert_equal((t4 + t3).direction.to_degrees(), 0);
	assert_equal((t4 + t3).vector.x, 0);
	assert_equal((t4 + t3).vector.y, 0);

	t4 = -t3;
	assert_equal((t3 + t4).direction.to_degrees(), 0);
	assert_equal((t3 + t4).vector.x, 0);
	assert_equal((t3 + t4).vector.y, 0);
}

void test_physics_utility() {
	test_vector();
	test_direction();
	test_transformator();
}
