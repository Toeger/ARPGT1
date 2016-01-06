#include <cassert>

#include "tester.h"
#include "collisions.h"
#include "physics_utility.h"

bool Tester::run()
{
	test_physics_utility();
	test_collisions();
	return true;
}
