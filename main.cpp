#include "ECS/common_components.h"
#include "ECS/entity.h"
#include "ECS/utility.h"
#include "GamePlay/map.h"
#include "Graphics/camera.h"
#include "Graphics/input_handler.h"
#include "Graphics/perlinnoise.h"
#include "Graphics/physicals.h"
#include "Graphics/terrain.h"
#include "Graphics/window.h"
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

//handle continuous actions
void handle_input(Input_handler &input){
	//player movement
	Physical::Vector player_movement;
	if (input.continuous_actions[Input_handler::Action::go_forward]){
		player_movement.y++;
	}
	if (input.continuous_actions[Input_handler::Action::go_backward]){
		player_movement.y--;
	}
	if (input.continuous_actions[Input_handler::Action::go_left]){
		player_movement.x--;
	}
	if (input.continuous_actions[Input_handler::Action::go_right]){
		player_movement.x++;
	}
	if (player_movement.length() > 0.5f){ //we have player movement
		player_movement /= player_movement.length();
		(*Player::player.get<Physical::DynamicBody<Physical::Circle>>()) += player_movement * Player::player.get<Common_components::Speed>()->speed;
	}
}

void setup_controls(Input_handler &input_handler, Camera &camera)
{
	//camera
	{
		constexpr float camera_speed = 1.f;
		auto assign_cam_key_action = [&input_handler, &camera](irr::EKEY_CODE key, Input_handler::Action action, float x, float y, float z){
			input_handler.key_action_map[key] = action;
			input_handler.instant_actions[action] = [&camera, x, y, z]{
				auto pos = camera.get_position();
				camera.set_position(pos[0] + x * camera_speed, pos[1] + z * camera_speed, pos[2] + y * camera_speed);
			};
		};
		assign_cam_key_action(irr::KEY_UP, Input_handler::Action::camera_forward, 0, 1, 0);
		assign_cam_key_action(irr::KEY_DOWN, Input_handler::Action::camera_backward, 0, -1, 0);
		assign_cam_key_action(irr::KEY_LEFT, Input_handler::Action::camera_left, -1, 0, 0);
		assign_cam_key_action(irr::KEY_RIGHT, Input_handler::Action::camera_right, 1, 0, 0);
		assign_cam_key_action(irr::KEY_PLUS, Input_handler::Action::camera_up, 0, 0, 1);
		assign_cam_key_action(irr::KEY_OEM_2, Input_handler::Action::camera_down, 0, 0, -1); //# key
		input_handler.mouse_wheel[Input_handler::Mouse_wheel_up] = Input_handler::Action::camera_zoom_in;
		input_handler.instant_actions[Input_handler::Action::camera_zoom_in] = [&camera]{
			camera.camera_height *= 0.9f;
		};
		input_handler.mouse_wheel[Input_handler::Mouse_wheel_down] = Input_handler::Action::camera_zoom_out;
		input_handler.instant_actions[Input_handler::Action::camera_zoom_out] = [&camera]{
			camera.camera_height /= 0.9f;
		};
	}
}

int main(){
	ON_SCOPE_EXIT(ECS::Entity::clear_all(););
	assert(Tester::run());

	auto &now =  std::chrono::high_resolution_clock::now;
	auto last_update_timepoint = now();

	Input_handler input_handler;
	Window window(input_handler);
	Camera camera(window);
	Terrain terrain(window, "Art/perlin_map.bmp", "Art/cobble_stone.png");

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
		std::normal_distribution<float> ypos_gen(map_height / 2, map_height / 10);
		std::normal_distribution<float> xpos_gen(map_width / 2, map_width / 10);
		//std::normal_distribution<float> xpos_gen(0, map_width / 100);
		//std::normal_distribution<float> ypos_gen(0, map_height / 100);
		do {
			static int try_count = 0;
			std::cout << "try number " << ++try_count << '\n' << std::flush;
			blocked = false;
			b.move(Physical::Vector{xpos_gen(rng) * Map::current_map->block_size, ypos_gen(rng) * Map::current_map->block_size}, check_blocked_function);
		} while(blocked);
		p.add(std::move(b));
		p.add(Common_components::Speed{50});
	}
	setup_controls(input_handler, camera);

	//Network::run();
	while (window.update()){
		//resolve logical frame
		while (now() - last_update_timepoint > Config::logical_frame_duration){
			//handle continuous input
			handle_input(input_handler);
			auto pos = p.get<Physical::DynamicBody<Physical::Circle>>()->get_current_transformator().vector;
			const auto &camera_height = camera.camera_height;
			camera.set_position(pos.x / Map::current_map->block_size, camera_height, pos.y / Map::current_map->block_size - camera_height / 2);
			camera.look_at(pos.x / Map::current_map->block_size, 0, pos.y / Map::current_map->block_size);
			//auto cam_pos = camera.get_position();
			//std::cerr << "campos: " << cam_pos[0] << '/' << cam_pos[1] << '/' << cam_pos[2] << '\n';
			//update logic
			last_update_timepoint += Config::logical_frame_duration;
			Network::update();
			update_logical_frame();
		}
	}
	//Network::stop();
}
