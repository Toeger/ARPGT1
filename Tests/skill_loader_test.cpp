#include "GamePlay/Skills/skill.h"
#include "External/LuaContext.hpp"
#include "skill_loader_test.h"

#include <boost/algorithm/string/join.hpp>
#include <sstream>
#include <string>

static void load_empty() {
	std::stringstream data{R"({})"};
	auto skills = Skills::load(data);
	assert(skills.size() == 0);
}

static void ignore_comments() {
	{
		std::stringstream data{R"({"comment":{}})"};
		auto skills = Skills::load(data);
		assert(skills.size() == 0);
	}
	{
		std::stringstream data{R"({"comment":{}, "comment":{}, "comment":{}, "comment":{}})"};
		auto skills = Skills::load(data);
		assert(skills.size() == 0);
	}
}

static void load_name() {
	std::vector<std::string> names{"name1", "long name with spaces and weird c̼̯ͨ̂͐̈͋͠ͅhͧ̿̿̐̈́ͯ̃a͈̙͕͍̳̥ͅr͙͍̳̜̞̦̊ͪͪ̋͊̋͝a̟̰̟̥̮̭͗͐c͔̘͈̠͓̖̱͋ͯ̀t̩͇͉̥͊̓̑͌͝e̎̍r̰ͯ̇̇ͦ̋̚s̱̻̬ͥ̍ͪ̿"};
	std::stringstream data;
	data << "{\"";
	data << boost::algorithm::join(names, "\":{},\""); //using boost out of laziness, could get rid of the dependency here
	data << "\":{}}";
	auto skills = Skills::load(data);
	assert(skills.size() == names.size());
	for (auto &skill : skills) {
		auto pos = find(begin(names), end(names), skill.name);
		assert(pos != end(names));
		names.erase(pos);
	}
	assert(names.size() == 0);
}

static void lua_oncreate_test() {
	static std::string test_write_string; //need to make this static so I can use it with the lambda without capturing it,
	test_write_string.clear();            //because capturing would prevent the lambda from being convertible to a function pointer
	std::stringstream data{R"xxx({"skillname" : {"oncreate":"test(\"success\")"}})xxx"};
	auto skill_definitions =
		Skills::load(data, [](LuaContext &context) { context.writeFunction("test", [](const std::string &s) { test_write_string = s; }); });
	assert(skill_definitions.size() == 1);
	auto &skill_definition = skill_definitions.front();
	assert(skill_definition.on_create);
	auto skill_instance = skill_definition.create();
	skill_instance.on_create();
	assert(test_write_string == "success");
}

void test_skill_loader() {
	load_empty();
	ignore_comments();
	load_name();
	lua_oncreate_test();
}
