#include "skill_loader_test.h"
#include "GamePlay/skill.h"

#include <sstream>

static void ignore_comments(){
	std::stringstream data;
	std::vector<Skills::Skill> skills;

	for (auto &d : {
		 "",
		 "/**/",
		 "\n/*{}dflakjf\n}*/\n  \t\n"}){
		data.str(d);
		skills = Skills::load(data);
		assert(skills.size() == 0);
	}
}

static void load_single_simple(){
	std::stringstream data{"{}"};
	auto skills = Skills::load(data);
	assert(skills.size() == 1);
}

void test_skill_loader()
{
	ignore_comments();
	load_single_simple();
}
