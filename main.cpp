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

int main()
{
	Physical::Body body;
	body.attach(Physical::Circle({10, 20}, 10), {0, 0}, {0, 0});

	sf::RenderWindow window(sf::VideoMode(800, 600), "ARPGT1");
	{ //this should not do anything, but somehow prevents the window from not responding
		sf::Event event;
		while (window.pollEvent(event));
	}
	auto &now =  std::chrono::high_resolution_clock::now;
	auto last_update_timepoint = now();
	const auto lfps = 30;
	const auto logical_frame_duration = std::chrono::milliseconds(1000) / lfps;
	using namespace std::chrono_literals;

	window.setVerticalSyncEnabled(true);

	Player &p = Player::player;
	p.set_window(&window);
	p.camera.rotate(0);
	p.camera.set_size(80, 60);
	p.camera.set_position(0, 0);

	std::vector<Entity> balls(5);
	{
		sf::CircleShape c;
		c.setFillColor(sf::Color::Red);
		c.setRadius(10);
		c.setPosition(-10, -10);
		balls[0].add(c);
		c.setPosition(-10, 10);
		balls[1].add(c);
		c.setPosition(10, 10);
		balls[2].add(c);
		c.setPosition(10, -10);
		balls[3].add(c);
		c.setPosition(5, 5);
		c.setFillColor(sf::Color::Green);
		balls[4].add(c);
	}

	while (window.isOpen()){
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed){
				p.set_window(nullptr);
				window.close();
			}
			if (event.type == sf::Event::Resized)
				//auto zoom = p.camera.get_zoom();
				p.camera.set_size(event.size.width, event.size.height);
				//p.camera.set_zoom(zoom);
				//window.setSize({event.size.width, event.size.height});
			if (event.type == sf::Event::MouseWheelMoved){
				const auto zoom_factor = 0.1f;
				p.camera.set_zoom(pow(1 - zoom_factor, event.mouseWheel.delta));
			}
		}
		if (!window.isOpen())
			continue;
		//UpdateList::updateAll();
		while (now() - last_update_timepoint > logical_frame_duration)
		{
			//resolve physics
			last_update_timepoint += logical_frame_duration;
			//setting player direction and velocity
			{
				if (window.hasFocus()){
					const auto camera_turning_speed = 5.f; //in degree
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
						p.camera.rotate(-camera_turning_speed);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
						p.camera.rotate(camera_turning_speed);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)){
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
						//for now shoot a ball straight up

					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
						auto &pos = balls[4].get<sf::CircleShape>()->getPosition();
						Physical::Vector v{pos.x, pos.y};
						Physical::Transformator t;
						t.set_translation({0, 1});
						v = t * v;
						balls[4].get<sf::CircleShape>()->setPosition(v.x, v.y);
					}
					//player_body->ApplyForce(vel, player_body->GetPosition(), true);
				}
			}

		}
		p.camera.set_position(0, 0);
		window.clear(sf::Color::Black);
		//rendering system
		for (auto sit = System::range<sf::Sprite>(); sit; sit.advance()){
			window.draw(sit.get<sf::Sprite>());
		}
		for (auto sit = System::range<sf::CircleShape>(); sit; sit.advance()){
			window.draw(sit.get<sf::CircleShape>());
		}
		{
			static int fps;
			fps++;
			static auto starttime = std::chrono::high_resolution_clock::now();
			const auto measure_time = std::chrono::milliseconds(500);
			if (std::chrono::high_resolution_clock::now() - starttime > measure_time){
				starttime += measure_time;
				window.setTitle("FPS: " + std::to_string(fps*2));
				fps = 0;
			}
		}
		Graphics::draw_physicals(window);
		window.display();
	}
}
