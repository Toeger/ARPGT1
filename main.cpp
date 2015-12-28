#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <memory>
#include <tuple>
#include <initializer_list>
#include <array>
#include <typeinfo>

#include "player.h"
#include "practicedummy.h"
#include "entity.h"
#include "utility.h"
#include "make_function.h"

#include "Physics/body.h"
#include "Physics/circle.h"
#include "Graphics/physicals.h"

void handle_events(sf::RenderWindow &window, Player &p){
	//check immediate action buttons
	if (window.hasFocus()){
		const auto camera_turning_speed = 5.f; //in degree
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
			p.camera.rotate(-camera_turning_speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
			p.camera.rotate(camera_turning_speed);
		}
	}

	//check all events that were triggered since the last iteration of the loop
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type){
		case sf::Event::Closed: //closed the window
		{
			p.set_window(nullptr);
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
		default:
		{
		}
		break;
		}
	}
}

void update_logical_frame(){
	//TODO: movement, collision detection, ...
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
}

int main(){
	Physical::Body body;
	body.attach(Physical::Circle({20, 10}, 100), {0, 0}, {0, 0});

	sf::RenderWindow window(sf::VideoMode(800, 600), "ARPGT1");
	{
		//this should not do anything, but somehow sometimes prevents the window from not responding
		//see http://en.sfml-dev.org/forums/index.php?topic=19513.0
		sf::Event event;
		while (window.pollEvent(event));
	}
	window.setVerticalSyncEnabled(true);

	auto &now =  std::chrono::high_resolution_clock::now;
	auto last_update_timepoint = now();
	const auto lfps = 30; //logical frames per second, independent of graphical fps
	const auto logical_frame_duration = std::chrono::milliseconds(1000) / lfps;


	Player &p = Player::player;
	p.set_window(&window);
	{
		Physical::Body b;
		b.attach(Physical::Circle(Physical::Transformator(), 100), {}, {});
		p.add(std::move(b));
	}

	while (window.isOpen()){
		while (now() - last_update_timepoint > logical_frame_duration){
			last_update_timepoint += logical_frame_duration;
			update_logical_frame();
		}
		p.camera.set_position(0, 0);
		render_frame(window);
		handle_events(window, p);
	}
}
