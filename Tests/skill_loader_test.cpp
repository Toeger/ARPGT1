#include "skill_loader_test.h"
#include "GamePlay/skill.h"

#include <boost/algorithm/string/join.hpp>
#include <sstream>

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

void test_skill_loader()
{
	load_empty();
	ignore_comments();
	load_name();
}
