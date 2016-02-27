#include "collisions.h"
#include "ecs_test.h"
#include "perlin_test.h"
#include "physics_utility.h"
#include "tester.h"

#include <cassert>

bool Tester::run()
{
	test_physics_utility();
	test_collisions();
	test_ecs();
	test_perlin();
	return true;
}
