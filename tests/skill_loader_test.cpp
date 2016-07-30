#include "gameplay/skills/skill.h"
#include "external/LuaContext.hpp"
#include "gameplay/map.h"
#include "graphics/window.h"
#include "main.h"
#include "physics/body.h"
#include "player.h"
#include "skill_loader_test.h"

#include <boost/algorithm/string/join.hpp>
#include <irrlicht/irrlicht.h>
#include <sstream>
#include <string>

static void load_empty() {
	std::stringstream data{R"({})"};
	auto skills = Skills::load(data);
	assert(skills.empty());
}

static void ignore_comments() {
	{
		std::stringstream data{R"({"comment":{}})"};
		auto skills = Skills::load(data);
		assert(skills.empty());
	}
	{
		std::stringstream data{R"({"comment":{}, "comment":{}, "comment":{}, "comment":{}})"};
		auto skills = Skills::load(data);
		assert(skills.empty());
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
	assert(names.empty());
}

static void lua_oncreate_test() {
	auto window = Window::get_dummy();
	Window::current_window = &window;
	ON_SCOPE_EXIT(Window::current_window = nullptr;);
	ECS::Entity map;
	Map::current_map = &map.emplace<Map>(100, 100);
	ON_SCOPE_EXIT(Map::current_map = nullptr;);
	Physical::DynamicBody<Physical::Circle> body{100};
	Player::player.add(std::move(body));
	ON_SCOPE_EXIT(Player::player.remove<decltype(body)>(););
	static std::string test_write_string; //need to make this static so I can use it with the lambda without capturing it,
	test_write_string.clear();            //because capturing would prevent the lambda from being convertible to a function pointer
	std::stringstream data{R"xxx({"skillname" :{
		"type" : "projectile",
		"oncreate":"test(\"success\")",
		"animation" : "art/circle.ms3d",
		"texture" : "art/circle.png"
		}})xxx"};
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
