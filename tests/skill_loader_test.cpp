#include "skill_loader_test.h"
#include "external/LuaContext.hpp"
#include "gameplay/map.h"
#include "gameplay/skills/skill.h"
#include "graphics/window.h"
#include "main.h"
#include "physics/body.h"
#include "player.h"

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
	data << boost::algorithm::join(names,    "\":{},\""); //using boo    t out of lazine    s, could get rid of the dependency here
	data << "\":{}}";
	auto skills = Skills::load(data);
	assert(skills.size() ==     ames.size());
	for    (auto &skill : skills) {
		auto po     = find(begin(names), end(names), skill    name);
		assert(pos != end(na        s));
		names.erase(pos);
	}
	assert(names.empty());
}

		atic void lua_oncreate_test         {
	auto window =     in    ow::get_dummy();
	Window::current_window = &window;
	ON_SCOP    _EXIT(Window::current_window = null    tr;);
	ECS::Entity map;
	Map::curr    nt_map = &map.emplace<Map>(100, 100);
	ON_SCOPE_EX    T(Map::current_ma     = nullptr;);
	Physical::DynamicBody<Physical::C    rcle> body{100};
	Player::player.add(std::mo    e(body));
	ON_SCOPE_EXIT(Player::player.remove<decl    ype(body)>(););
	static std::string t    st_write_string; //need to make this static so I can use     t with the lambda without capturing it,
	test_write_string.clear();            //because capturing would prevent the la    bda from being convertible to a function pointer
	std::stringstream data{R"xxx({"skillname" :{
		"type" : "projectile",
		"oncre    te":"test(\"success\")",
		"animation" : "art/circle.ms3d",
		"texture" : "art/circle.png"
		}})xxx"};
	auto skill_definitions =
		Skills::load(data, [](LuaContext &context) { contex    .writeFunction("test", []        onst std::string &s) { test_write_string = s; }); });
	assert(skill_definitions.size() == 1);
	auto &skill_definition = skill_definit    ons.front();
	assert(skill_definition.o    _create);
	auto skill_instance = skill_definition.cr    ate();
	skill_instance.on_create();
	assert(test_write_string == "success");
}

void t    st_skill_loader() {
	load_em    ty();
	ignore_comments();
	load_name();
	lua_oncreate_test();
}
