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
	//run ECS systems
	Physical::end_frame();
	check_remove_automatic_entities();
	ECS::System::run_systems();
}

struct Input_handler : public irr::IEventReceiver{
	//set up some reasonable action map
	//TODO: overwrite with settings
	Input_handler(){
		//WASD movement
		action_map[irr::KEY_KEY_W] = Action::go_forward;
		action_map[irr::KEY_KEY_A] = Action::go_left;
		action_map[irr::KEY_KEY_S] = Action::go_backward;
		action_map[irr::KEY_KEY_D] = Action::go_right;
		//arrow key movement
		action_map[irr::KEY_UP] = Action::go_forward;
		action_map[irr::KEY_LEFT] = Action::go_left;
		action_map[irr::KEY_DOWN] = Action::go_backward;
		action_map[irr::KEY_RIGHT] = Action::go_right;
	}
	//logical actions that a character can perform
	enum Action : unsigned char{
		none,
		go_forward,
		go_backward,
		go_left,
		go_right,
		size
	};
	//defines the action for discrete events. Discrete events are activated once by a keypress
	Action action_map[irr::KEY_KEY_CODES_COUNT] = {};
	//defines which continuous actions are happening. Continuous actions are happening when a key is pressed
	bool continuous_actions[Action::size] = {};
	//defines which function is called when an action occures
	void (*instant_actions[Action::size])() = {};
	//handle continuous actions
	void handle_input(){
		//player movement
		Physical::Vector player_movement;
		if (continuous_actions[Action::go_forward]){
			player_movement.y++;
		}
		if (continuous_actions[Action::go_backward]){
			player_movement.y--;
		}
		if (continuous_actions[Action::go_left]){
			player_movement.x--;
		}
		if (continuous_actions[Action::go_right]){
			player_movement.x++;
		}
		if (player_movement.length() > 0.5f){ //we have player movement
			player_movement /= player_movement.length();
			(*Player::player.get<Physical::DynamicBody<Physical::Circle>>()) += player_movement * Player::player.get<Common_components::Speed>()->speed;
		}
	}
private:
	bool OnEvent(const irr::SEvent& event) override{
		if (event.EventType == irr::EET_KEY_INPUT_EVENT){
			//set what actions are happening
			continuous_actions[action_map[event.KeyInput.Key]] = event.KeyInput.PressedDown;
			//call instant action function if set
			auto f = instant_actions[action_map[event.KeyInput.Key]];
			if (f){
				f();
			}
		}
		return false;
	}
};

int main(){
	ON_SCOPE_EXIT(ECS::Entity::clear_all(););
	assert(Tester::run());

	//setup irrlicht graphics
	Input_handler input_handler;
	auto render_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(800, 600), 32, false, false, true, &input_handler);
	assert(render_device);
	ON_SCOPE_EXIT(render_device->drop(););
	render_device->setWindowCaption(L"ARPGT1");
	auto video_driver = render_device->getVideoDriver();
	auto scene_manager = render_device->getSceneManager();
	//auto camera = scene_manager->addCameraSceneNodeFPS(0,100.0f,1.2f);
	auto camera = scene_manager->addCameraSceneNode();
	camera->setFarValue(100000.0f);
	auto terrain = scene_manager->addTerrainSceneNode(
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
	terrain->scaleTexture(10.f, 1.f);

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
		//resolve logical frame
		while (now() - last_update_timepoint > Config::logical_frame_duration){
			//handle continuous input
			input_handler.handle_input();
			auto pos = p.get<Physical::DynamicBody<Physical::Circle>>()->get_current_transformator().vector;
			camera->setPosition({pos.x, 20000, pos.y - 1000});
			camera->setTarget({pos.x, 0, pos.y});
			std::cerr << pos.x << ' ' << pos.y << '\n';
			//update logic
			last_update_timepoint += Config::logical_frame_duration;
			Network::update();
			update_logical_frame();
		}
	}
	//Network::stop();
}
