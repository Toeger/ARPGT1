#include "skill_loader_test.h"
#include "GamePlay/skill.h"

#include <sstream>

static void load_empty(){
	std::stringstream data{R"({})"};
	auto skills = Skills::load(data);
	assert(skills.size() == 0);
}

void test_skill_loader()
{
	load_empty();
}
