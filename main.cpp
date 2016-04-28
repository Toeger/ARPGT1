#include "main.h"
#include "ECS/common_components.h"
#include "ECS/entity.h"
#include "ECS/utility.h"
#include "GamePlay/map.h"
#include "Graphics/camera.h"
#include "Graphics/common_graphics_components.h"
#include "Graphics/input_handler.h"
#include "Graphics/perlinnoise.h"
#include "Graphics/physicals.h"
#include "Graphics/terrain.h"
#include "Graphics/window.h"
#include "Physics/body.h"
#include "Physics/sensor.h"
#include "Physics/shapes.h"
#include "Tests/tester.h"
#include "make_function.h"
#include "network.h"
#include "player.h"

#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <random>
#include <tuple>
#include <type_traits>
#include <typeinfo>

Window *Global::window;

namespace {
	std::mt19937_64 rng(std::random_device{}());
}

template <class T>
std::enable_if_t<std::is_floating_point<T>::value, T> get_random_number(T from, T to) {
	return std::uniform_real_distribution<T>(from, to)(rng);
}
template <class T>
std::enable_if_t<!std::is_floating_point<T>::value, T> get_random_number(T from, T to) {
	return std::uniform_int_distribution<T>(from, to)(rng);
}

void debug_print(const Physical::Circle &c, const Physical::Transformator &t) {
	std::cout << "circle: x: " << t.vector.x << " y: " << t.vector.y << " r: " << c.radius << '\n';
}

void debug_print(const Physical::Line &l, const Physical::Transformator &t) {
	auto pos = t + l.vector;
	std::cout << "line: x1: " << t.vector.x << " y1: " << t.vector.y << " x2: " << pos.vector.x << " y2: " << pos.vector.y << '\n';
}

//remove temporary entities
static void check_remove_automatic_entities() {
	auto &removers = ECS::System::get_components<ECS::Remove_checker>();
	for (std::size_t i = 0; i < removers.size();) { //using indexes because iterators become invalid
		if (removers[i].function(removers[i].entity.to_handle())) {
			removers.erase(begin(removers) + i);
		} else {
			i++;
		}
	}
}

static void update_logical_frame() {
	//run ECS systems
	Physical::end_frame();
	check_remove_automatic_entities();
	ECS::System::run_systems();
}

//handle continuous actions
void handle_input(Input_handler &input, Camera &camera) {
	//player movement
	{
		int x = 0;
		int y = 0;
		if (input.is_action_happening(Input_handler::Action::go_forward)) {
			y++;
		}
		if (input.is_action_happening(Input_handler::Action::go_backward)) {
			y--;
		}
		if (input.is_action_happening(Input_handler::Action::go_left)) {
			x++;
		}
		if (input.is_action_happening(Input_handler::Action::go_right)) {
			x--;
		}
		if (x | y) { //we have player movement
			Physical::Direction move_direction = Physical::Direction(x, y) - camera.get_direction() - Physical::Direction{0, 1};
			(*Player::player.get<Physical::DynamicBody<Physical::Circle>>()) +=
				Physical::Vector{move_direction.get_x(), move_direction.get_y()} * Player::player.get<Common_components::Speed>()->speed;
		}
	}
	if (input.is_action_happening(Input_handler::Action::camera_rotate_clockwise)) {
		camera.turn_clockwise();
	}
	if (input.is_action_happening(Input_handler::Action::camera_rotate_counterclockwise)) {
		camera.turn_counterclockwise();
	}
}

void setup_controls(Input_handler &input_handler, Camera &camera) {
	//camera
	{
		input_handler.mouse_wheel[Input_handler::Mouse_wheel_up] = Input_handler::Action::camera_zoom_in;
		input_handler.instant_actions[Input_handler::Action::camera_zoom_in] = [&camera] { camera.zoom_in(); };
		input_handler.mouse_wheel[Input_handler::Mouse_wheel_down] = Input_handler::Action::camera_zoom_out;
		input_handler.instant_actions[Input_handler::Action::camera_zoom_out] = [&camera] { camera.zoom_out(); };
		input_handler.key_action_map[irr::KEY_KEY_Q] = Input_handler::camera_rotate_clockwise;
		input_handler.key_action_map[irr::KEY_KEY_E] = Input_handler::camera_rotate_counterclockwise;
	}
	//skills
	{
		input_handler.key_action_map[irr::KEY_SPACE] = Input_handler::cast_skill_1;
		input_handler.key_action_map[irr::KEY_ESCAPE] = Input_handler::interrupt_cast;
		input_handler.instant_actions[Input_handler::cast_skill_1] = [] {
			auto &p = Player::player;
			p.get<std::vector<Skills::Skill_instance>>()->emplace_back(p.skills[1].create());
		};
	}
}

void light_controls(Input_handler &input_handler, Camera &camera) {
	constexpr float light_speed = 10.f;
	auto assign_light_key_action = [&input_handler, &camera](irr::EKEY_CODE key, Input_handler::Action action, float x, float y, float z) {
		input_handler.key_action_map[key] = action;
		input_handler.instant_actions[action] = [&camera, x, y, z] {
			auto pos = camera.get_light_position();
			std::cerr << pos[0] << ' ' << pos[1] << ' ' << pos[2] << '\n';
			camera.set_light_position(pos[0] + x * light_speed, pos[1] + y * light_speed, pos[2] + z * light_speed);
		};
	};
	assign_light_key_action(irr::EKEY_CODE::KEY_KEY_U, Input_handler::Action::light_right, 1, 0, 0);
	assign_light_key_action(irr::EKEY_CODE::KEY_KEY_J, Input_handler::Action::light_left, -1, 0, 0);
	assign_light_key_action(irr::EKEY_CODE::KEY_KEY_I, Input_handler::Action::light_up, 0, 1, 0);
	assign_light_key_action(irr::EKEY_CODE::KEY_KEY_K, Input_handler::Action::light_down, 0, -1, 0);
	assign_light_key_action(irr::EKEY_CODE::KEY_KEY_O, Input_handler::Action::light_forward, 0, 0, 1);
	assign_light_key_action(irr::EKEY_CODE::KEY_KEY_L, Input_handler::Action::light_backward, 0, 0, -1);
}

int main() {
	assert(Tester::run());
	//return 0;

	auto &now = std::chrono::high_resolution_clock::now;
	auto last_update_timepoint = now();

	Input_handler input_handler;
	Window window(input_handler);
	Global::window = &window;
	Camera camera(window);
	ECS::Entity map;
	{
		constexpr std::size_t map_width = 1024;
		constexpr std::size_t map_height = 1024;
		Map::current_map = &map.emplace<Map>(map_width, map_height);
		map.emplace<Common_components::Map>();
	}
	Terrain terrain(window, *Map::current_map, "Art/cobble_stone.png");

	{ //add running straight AI system
		auto fun = [](ECS::Entity_handle monster, Physical::Transformator &player_pos) {
			auto &monster_body = *monster.get<Physical::DynamicBody<Physical::Circle>>();
			auto monster_transformator = monster_body.get_current_transformator();
			auto to_player_vector = player_pos.vector - monster_transformator.vector;
			auto monster_speed = monster.get<Common_components::Speed>()->speed;
			monster_body += Physical::Direction{to_player_vector.x, to_player_vector.y} - monster_transformator.direction;
			monster_body += monster_speed;
		};

		auto precomputer = [] { return Player::player.get<Physical::DynamicBody<Physical::Circle>>()->get_current_transformator(); };

		ECS::System::add_system<Common_components::Run_straight_AI>(fun, precomputer);
	}

	{ //add skill system
		auto fun = [](ECS::Entity_handle character) {
			auto skill = character.get<Skills::Skill_instance>();
			skill->on_tick();
		};

		ECS::System::add_system<Skills::Skill_instance>(fun);
	}

	Player &p = Player::player;
	{
		Physical::DynamicBody<Physical::Circle> b{100};
		bool blocked = false;
		auto check_blocked_function = [&blocked](const Physical::Transformator &, ECS::Entity_handle) {
			blocked = true;
			return Physical::Vector{};
		};
		const auto &map_height = Map::current_map->get_height();
		const auto &map_width = Map::current_map->get_width();
		std::normal_distribution<float> ypos_gen(map_height / 2, map_height / 10);
		std::normal_distribution<float> xpos_gen(map_width / 2, map_width / 10);
		do {
			static int try_count = 0;
			std::cout << "try number " << ++try_count << '\n' << std::flush;
			blocked = false;
			const auto block_size = Map::current_map->get_block_size();
			b.move(Physical::Vector{xpos_gen(rng) * block_size, ypos_gen(rng) * block_size}, check_blocked_function);
		} while (blocked);
		p.add(std::move(b));
		p.add(Common_components::Speed{150});
		std::ifstream f("Data/skills.json");
		assert_fast(f);
		p.skills = Skills::load(f);
		p.emplace<std::vector<Skills::Skill_instance>>();
	}
	p.emplace<Common_components::Animated_model>(window, "Art/circle.ms3d", "Art/circle.png");
	setup_controls(input_handler, camera);

	//Network::run();
	while (window.update(camera)) {
		//resolve logical frame
		while (now() - last_update_timepoint > Config::logical_frame_duration) {
			//handle continuous input
			handle_input(input_handler, camera);
			const auto block_size = Map::current_map->get_block_size();
			auto pos = p.get<Physical::DynamicBody<Physical::Circle>>()->get_current_transformator().vector;
			auto xpos = Map::current_map->get_width() - 1 - pos.x / block_size;
			auto ypos = pos.y / block_size;
			camera.set_position(xpos, ypos);
			camera.look_at(xpos, 0, ypos);
			auto &player_animation = *p.get<Common_components::Animated_model>();
			player_animation.set_position(xpos, 0, ypos);
			//player_animation.look_at(xpos, ypos + 1);
			player_animation.set_rotation(std::chrono::high_resolution_clock::now().time_since_epoch().count() / 10000000 % 360);
			//update logic
			last_update_timepoint += Config::logical_frame_duration;
			Network::update();
			update_logical_frame();
		}
	}
	//Network::stop();
	ECS::Entity::clear_all();
}
