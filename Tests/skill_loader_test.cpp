#include "GamePlay/Skills/skill.h"
#include "skill_loader_test.h"
#include "External/LuaContext.hpp"

#include <boost/algorithm/string/join.hpp>
#include <sstream>
#include <string>

static void load_empty(){
	std::stringstream data{R"({})"};
	auto skills = Skills::load(data);
	assert(skills.size() == 0);
}

static void ignore_comments(){
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

static void load_name(){
	std::vector<std::string> names{"name1", "long name with spaces and weird c̼̯ͨ̂͐̈͋͠ͅhͧ̿̿̐̈́ͯ̃a͈̙͕͍̳̥ͅr͙͍̳̜̞̦̊ͪͪ̋͊̋͝a̟̰̟̥̮̭͗͐c͔̘͈̠͓̖̱͋ͯ̀t̩͇͉̥͊̓̑͌͝e̎̍r̰ͯ̇̇ͦ̋̚s̱̻̬ͥ̍ͪ̿"};
	std::stringstream data;
	data << "{\"";
	data << boost::algorithm::join(names, "\":{},\"");
	data << "\":{}}";
	//std::cerr << ">>>" << data.str() << "<<<\n";
	auto skills = Skills::load(data);
	assert(skills.size() == names.size());
	for (auto &skill : skills){
		auto pos = find(begin(names), end(names), skill.name);
		assert(pos != end(names));
		names.erase(pos);
	}
	assert(names.size() == 0);
}

std::string test_write_string;
static void lua_test(){
	std::stringstream data{R"xxx({"skillname" : {"oncreate":"test(\"success\")"}})xxx"};
	assert(test_write_string.empty());
	auto skills = Skills::load(data, [](LuaContext &context){
		context.writeFunction("test", [](const std::string &s){
		test_write_string = s;
		});
	});
	assert(skills.size() == 1);
	auto &skill = skills.front();
	assert(skill.oncreate);
	skill.oncreate(skill);
	assert(test_write_string == "success");
}

void test_skill_loader()
{
	load_empty();
	ignore_comments();
	load_name();
	lua_test();
}
