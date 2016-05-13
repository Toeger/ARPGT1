#include "tester.h"
#include "collisions.h"
#include "ecs_test.h"
#include "perlin_test.h"
#include "physics_utility.h"
#include "skill_loader_test.h"

bool Tester::run() {
	test_physics_utility();
	test_collisions();
	test_ecs();
	test_perlin();
	test_skill_loader();
	return true;
}
