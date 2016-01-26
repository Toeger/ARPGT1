#include <cassert>

#include "tester.h"
#include "collisions.h"
#include "physics_utility.h"
#include "ecs_test.h"

bool Tester::run()
{
	test_physics_utility();
	test_collisions();
	test_ecs();
	return true;
}
