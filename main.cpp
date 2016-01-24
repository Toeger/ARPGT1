#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <memory>
#include <tuple>
#include <initializer_list>
#include <array>
#include <typeinfo>
#include <cassert>

#include "player.h"
#include "practicedummy.h"
#include "ECS/entity.h"
#include "utility.h"
#include "make_function.h"

#include "Physics/body.h"
#include "Physics/sensor.h"
#include "Physics/shapes.h"
#include "Graphics/physicals.h"
#include "Tests/tester.h"

void debug_print(const Physical::Circle &c, const Physical::Transformator &t){
	std::cout << "circle: x: " << t.vector.x << " y: " << t.vector.y << " r: " << c.radius << '\n';
}

void debug_print(const Physical::Line &l, const Physical::Transformator &t){
	auto pos = t + l.vector;
	std::cout << "line: x1: " << t.vector.x << " y1: " << t.vector.y << " x2: " << pos.vector.x << " y2: " << pos.vector.y << '\n';
}

void handle_events(sf::RenderWindow &window){
	auto &p = Player::player;
	//check immediate action buttons
	p.move_offset.x = 0;
	p.move_offset.y = 0;
	p.turn_offset = Physical::Direction();
	if (window.hasFocus()){
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
			p.turn_offset += p.turn_speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
			p.turn_offset -= p.turn_speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			p.move_offset.y += 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
			p.move_offset.x -= 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			p.move_offset.y -= 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			p.move_offset.x += 1;
		}
	}

	//check all events that were triggered since the last iteration of the loop
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type){
		case sf::Event::Closed: //closed the window
		{
			window.close();
		}
		return;
		case sf::Event::Resized:
		{
			//auto zoom = p.camera.get_zoom();
			p.camera.set_size(event.size.width, event.size.height);
			//p.camera.set_zoom(zoom);
			//window.setSize({event.size.width, event.size.height});
		}
		break;
		case sf::Event::MouseWheelMoved:
		{
			const auto zoom_factor = 0.1f;
			p.camera.set_zoom(pow(1 - zoom_factor, event.mouseWheel.delta));
		}
		break;
		case sf::Event::KeyPressed:
		{
			switch (event.key.code){
			case sf::Keyboard::Escape:
				window.close();
			return;
			case sf::Keyboard::L:
			{
				//Debug: print out all physical entities
				Physical::apply_to_physical_bodies([](auto &body){
					debug_print(body.get_shape(), body.get_current_transformator());
				});
				std::cout << std::flush;
			}
			break;
			case sf::Keyboard::Space:
			{
				//TODO: build real skill system and map keys to logical actions
				//shoot a ball from the player in the facing direction
				//standard projectile properties: range, Physical::Body, direction, speed, destruction when range ran out or something was hit, coolddown
				//need to get the physics system to give me the entity that got hit
				ECS::Entity ball;
				auto transformator = Player::player.get<Physical::DynamicBody<Physical::Circle>>()->get_current_transformator();
				const auto ball_radius = 10;
				transformator += Physical::Vector{0, 100 + ball_radius + 10}; //player radius + ball radius //TODO: ask the player for its radius
				struct Life_time{
					int logical_frames_left;
				};
				struct Speed{
					float speed;
				};

				ball.add(Life_time{100});
				ball.add(Speed{Player::player.move_speed * 1.1f});
				//Physical::Sensor body(transformator);
				//body.attach(Physical::Circle(20));
				//ball.add(std::move(body));
				std::move(ball).make_automatic([](ECS::Entity &ball)
				{
					//(*ball.get<Physical::Sensor>()) += Physical::Vector(0, ball.get<Speed>()->speed);
					return !ball.get<Life_time>()->logical_frames_left--;
				});
			}
			break;
			default:
			break;
			}
		}
		break;
		default:
		break;
		}
	}
}

//remove temporary entities
void check_remove_automatic_entities(){
	auto &removers = ECS::System::get_components<ECS::Remove_checker>();
	for (auto it = begin(removers); it != end(removers);){
		if (it->function(it->entity)){
			it = removers.erase(it);
		}
		else{
			++it;
		}
	}
}

void update_logical_frame(){
	//movement, collision detection, ...
	auto &p = Player::player;
	auto length = p.move_offset.length();
	auto &playerbody = *p.get<Physical::DynamicBody<Physical::Circle>>();
	if (length){
		p.move_offset *= p.move_speed / length;
		playerbody += p.move_offset;
	}
	playerbody += p.turn_offset;
	Physical::end_frame();
	check_remove_automatic_entities();
}

void render_frame(sf::RenderWindow &window){
	window.clear(sf::Color::Black);
	{
		static int fps;
		fps++;
		static auto starttime = std::chrono::high_resolution_clock::now();
		const auto measure_time = std::chrono::milliseconds(500);
		if (std::chrono::high_resolution_clock::now() - starttime > measure_time){
			starttime += measure_time;
			window.setTitle("FPS: " + std::to_string(fps * 1000 / measure_time.count()));
			fps = 0;
		}
	}
	Graphics::draw_physicals(window);
	window.display();
	//window.display(); //add this to remove double image issues and half the framerate
}

int main(){
	ON_SCOPE_EXIT(ECS::Entity::clear_all(););
	assert(Tester::run());
	sf::RenderWindow window(sf::VideoMode(800, 600), "ARPGT1");
	{
		//this should not do anything, but somehow sometimes prevents the window from not responding
		//see http://en.sfml-dev.org/forums/index.php?topic=19513.0
		sf::Event event;
		while (window.pollEvent(event));
	}
	window.setVerticalSyncEnabled(false);

	auto &now =  std::chrono::high_resolution_clock::now;
	auto last_update_timepoint = now();
	const auto lfps = 30; //logical frames per second, independent of graphical fps
	const auto logical_frame_duration = std::chrono::milliseconds(1000) / lfps;

	ECS::Entity dots[4];
	{
		int counter = 0;
		Physical::Vector ps[] = {{1000, 1000}, {-1000, 1000}, {1000, -1000}, {-1000, -1000}};
		for (auto &p : ps){
			Physical::DynamicBody<Physical::Circle> b(Physical::Circle(10), p);
			dots[counter++].add(std::move(b));
		}
	}

	/*
	Entity temp;
	{
		Physical::Body b;
		b.attach(Physical::Circle(10), {1000, 1000}, {});
		b.attach(Physical::Circle(10), {-1000, 1000}, {});
		b.attach(Physical::Circle(10), {1000, -1000}, {});
		b.attach(Physical::Circle(10), {-1000, -1000}, {});
		temp.add(std::move(b));
	}
	*/

	Player &p = Player::player;
	p.set_window(&window);
	{
		Physical::DynamicBody<Physical::Circle> b{Physical::Circle(100)};
		p.add(std::move(b));
	}

	while (window.isOpen()){
		while (now() - last_update_timepoint > logical_frame_duration){
			last_update_timepoint += logical_frame_duration;
			update_logical_frame();
		}
		p.center_camera();
		render_frame(window);
		handle_events(window);
	}
	p.set_window(nullptr); //prevent the camera from crashing due to not having a window
}
