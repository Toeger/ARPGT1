#include "ECS/common_components.h"
#include "ECS/entity.h"
#include "ECS/utility.h"
#include "GamePlay/map.h"
#include "Graphics/perlinnoise.h"
#include "Graphics/physicals.h"
#include "make_function.h"
#include "network.h"
#include "Physics/body.h"
#include "Physics/sensor.h"
#include "Physics/shapes.h"
#include "player.h"
#include "Tests/tester.h"

#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <initializer_list>
#include <iostream>
#include <irrlicht/irrlicht.h>
#include <memory>
#include <random>
#include <tuple>
#include <typeinfo>
#include <type_traits>

namespace {
	std::mt19937 rng(std::random_device{}());
	ECS::Entity map;
}

template <class T>
std::enable_if_t<std::is_floating_point<T>::value, T>
get_random_number(T from, T to){
	return std::uniform_real_distribution<T>(from, to)(rng);
}
template <class T>
std::enable_if_t<!std::is_floating_point<T>::value, T>
get_random_number(T from, T to){
	return std::uniform_int_distribution<T>(from, to)(rng);
}

void debug_print(const Physical::Circle &c, const Physical::Transformator &t){
	std::cout << "circle: x: " << t.vector.x << " y: " << t.vector.y << " r: " << c.radius << '\n';
}

void debug_print(const Physical::Line &l, const Physical::Transformator &t){
	auto pos = t + l.vector;
	std::cout << "line: x1: " << t.vector.x << " y1: " << t.vector.y << " x2: " << pos.vector.x << " y2: " << pos.vector.y << '\n';
}

namespace Config{
	const auto lfps = 30; //logical frames per second, independent of graphical fps
	const auto logical_frame_duration = std::chrono::milliseconds(1000) / lfps;
}

//remove temporary entities
static void check_remove_automatic_entities(){
	auto &removers = ECS::System::get_components<ECS::Remove_checker>();
	for (std::size_t i = 0; i < removers.size();){ //using indexes because iterators become invalid
		if (removers[i].function(removers[i].entity.to_handle())){
			removers.erase(begin(removers) + i);
		}
		else{
			i++;
		}
	}
}

static void update_logical_frame(){
	//movement, collision detection, ...
	auto &p = Player::player;
	auto length = p.move_offset.length();
	auto &playerbody = *p.get<Physical::DynamicBody<Physical::Circle>>();
	auto &player_speed = p.get<Common_components::Speed>()->speed;
	if (length){
		p.move_offset *= player_speed / length;
		playerbody += p.move_offset;
	}
	playerbody += p.turn_offset;
	//playerbody += Physical::Direction(p.move_offset.x, p.move_offset.y);
	Physical::end_frame();
	check_remove_automatic_entities();
	ECS::System::run_systems();
}

int main(){
	ON_SCOPE_EXIT(ECS::Entity::clear_all(););
	assert(Tester::run());

	//setup irrlicht graphics
	auto render_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(800, 600), 32, false, false, true, nullptr);
	assert(render_device);
	ON_SCOPE_EXIT(render_device->drop(););
	render_device->setWindowCaption(L"ARPGT1");
	auto video_driver = render_device->getVideoDriver();
	auto scene_manager = render_device->getSceneManager();
	auto camera = scene_manager->addCameraSceneNodeFPS(0,100.0f,1.2f);
	camera->setPosition(irr::core::vector3df(2700*2,255*10,2600*2));
	camera->setTarget(irr::core::vector3df(2397*2,343*2,2700*2));
	camera->setFarValue(42000.0f);
	irr::scene::ITerrainSceneNode* terrain = scene_manager->addTerrainSceneNode(
				"Art/perlin_map.bmp",
				0,											//parent node
				-1,											//node id
				irr::core::vector3df(0.f, 0.f, 0.f),		//position
				irr::core::vector3df(0.f, 0.f, 0.f),		//rotation
				irr::core::vector3df(100.f, 50.f, 100.f),	//scale
				irr::video::SColor ( 255, 255, 255, 255 ),	//vertexColor
				5,											//maxLOD
				irr::scene::ETPS_9,							//patchSize
				0											//smoothFactor
				);
	terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0, video_driver->getTexture("Art/wood-rotten.png"));
	terrain->setMaterialTexture(1, video_driver->getTexture("Art/Parchment.jpg"));
	terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
	terrain->scaleTexture(1.0f, 20.0f);

	auto &now =  std::chrono::high_resolution_clock::now;
	auto last_update_timepoint = now();

	{ //add running straight AI system
		auto fun = [](ECS::Entity_handle monster, Physical::Transformator &player_pos){
			auto &monster_body = *monster.get<Physical::DynamicBody<Physical::Circle>>();
			auto monster_transformator = monster_body.get_current_transformator();
			auto to_player_vector = player_pos.vector - monster_transformator.vector;
			auto monster_speed = monster.get<Common_components::Speed>()->speed;
			monster_body += Physical::Direction{to_player_vector.x, to_player_vector.y} - monster_transformator.direction;
			monster_body += monster_speed;
		};

		auto precomputer = []{
			return Player::player.get<Physical::DynamicBody<Physical::Circle>>()->get_current_transformator();
		};

		ECS::System::add_system<Common_components::Run_straight_AI>(fun, precomputer);
	}

	constexpr std::size_t map_width = 1024;
	constexpr std::size_t map_height = 1024;
	Map::current_map = &map.emplace<Map>(map_width, map_height);
	map.emplace<Common_components::Map>();

	Player &p = Player::player;
	{
		Physical::DynamicBody<Physical::Circle> b{100};
		bool blocked = false;
		auto check_blocked_function = [&blocked](const Physical::Transformator &, ECS::Entity_handle){
			blocked = true;
			return Physical::Vector{};
		};
		std::normal_distribution<float> xpos_gen(map_width / 2, map_width / 10);
		std::normal_distribution<float> ypos_gen(map_height / 2, map_height / 10);
		do {
			static int try_count = 0;
			std::cout << "try number " << ++try_count << '\n' << std::flush;
			blocked = false;
			b.move(Physical::Vector{xpos_gen(rng) * Map::current_map->block_size, ypos_gen(rng) * Map::current_map->block_size}, check_blocked_function);
		} while(blocked);
		p.add(std::move(b));
		p.add(Common_components::Speed{50});
	}

	//Network::run();
	while (render_device->run()){
		//render
		video_driver->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
		scene_manager->drawAll();
		video_driver->endScene();
		//resolve physics
		while (now() - last_update_timepoint > Config::logical_frame_duration){
			last_update_timepoint += Config::logical_frame_duration;
			Network::update();
			update_logical_frame();
		}
	}
	//Network::stop();
}
