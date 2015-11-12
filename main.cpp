#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <memory>
#include <tuple>
#include <initializer_list>
#include <array>
#include <typeinfo>
#include <Box2D/Box2D.h>

#include "player.h"
#include "practicedummy.h"
#include "entity.h"
#include "components.h"
#include "utility.h"
#include "zombieai.h"
#include "physical.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "ARPGT1");
	auto &now =  std::chrono::high_resolution_clock::now;
	auto last_update_timepoint = now();
	const auto lfps = 30;
	const auto logical_frame_duration = std::chrono::milliseconds(1000) / lfps;
	using namespace std::chrono_literals;

	window.setVerticalSyncEnabled(true);

	b2World world({0, 0});
	Physical::world = &world;
	/*TODO:
	ON_SCOPE_EXIT(
		System::clear<Components::PhysicalCircle>();
	);
	*/

	Player &p = Player::player;
	p.set_window(&window);
	Components::add_PhysicalCircleShape(p, 50/100.f, {10, 10}, sf::Color::Blue);
	p.camera.rotate(0);

	PracticeDummy pd;
	Components::add_PhysicalCircleShape(pd, 40/100.f, {10, 15}, sf::Color::White);
	sf::CircleShape s;
	std::vector<Entity> zombies(32);
	int zcounter = 0;
	for (auto &z : zombies){
		Components::add_PhysicalCircleShape(z, 40/100.f, {zcounter % 100, -zcounter / 100}, sf::Color::Red);
		z.emplace<Components::ZombieAi>();
		zcounter++;
	}
	Entity background_picture;
	{
		sf::Sprite sprite;
		background_picture.emplace<sf::Texture>();
		auto &image = *background_picture.get<sf::Texture>();
		sprite.setPosition(-1000000, -1000000);
		image.loadFromFile("grass3.png");
		sprite.setTexture(image);
		image.setRepeated(true);
		image.setSmooth(true);
		sprite.setTextureRect({0, 0, 2000000, 2000000});
		background_picture.add(std::move(sprite));
	}
#if 0
	sf::Font font;
	(void)font;

	if (!font.loadFromFile("Arctik 1.5.ttf"))
		throw std::runtime_error("Missing font: Arctik 1.5.ttf");
	sf::Text text;
	text.setFont(font);
#endif
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
			world.Step(1/30.f, 6, 2);
			last_update_timepoint += logical_frame_duration;
			auto &player_body = *p.get<Components::PhysicalCircle>();
			//setting player direction and velocity
			{
				b2Vec2 vel{0, 0};
				if (window.hasFocus()){
					const auto camera_turning_speed = 5.f; //in degree
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
						vel.y--;
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
						vel.x--;
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
						vel.y++;
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
						vel.x++;
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
						p.camera.rotate(-camera_turning_speed);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
						p.camera.rotate(camera_turning_speed);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)){
						const auto &dummy_body = *pd.get<Components::PhysicalCircle>();
						const auto &dummy_pos = dummy_body->GetPosition();
						const auto &player_pos = player_body->GetPosition();
						p.camera.face(dummy_pos.x - player_pos.x, dummy_pos.y - player_pos.y);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
						//for now shoot a ball straight up

					}
					vel.Normalize();
					vel *= 5;
					Utility::rotate(vel.x, vel.y, p.camera.get_rotation() * M_PI / 180);
					//player_body->ApplyForce(vel, player_body->GetPosition(), true);
				}
				player_body->SetLinearVelocity(vel);
			}
		}
		window.clear(sf::Color::Black);
		//rendering system
		ZombieAI::ZombieAI_system::update();
		for (auto sit = System::range<sf::Sprite>(); sit; sit.advance()){
			window.draw(sit.get<sf::Sprite>());
		}
		auto &player_body = *p.get<Components::PhysicalCircle>();
		const auto &pos = player_body->GetPosition();
		p.camera.set_position(pos.x * 100, pos.y * 100);
		for (auto sit = System::range<Components::CircleShape, Components::PhysicalCircle>(); sit; sit.advance()){
			const auto &pos = sit.get<Components::PhysicalCircle>()->GetPosition();
			auto &cs = sit.get<Components::CircleShape>();
			auto sfmlpos = Utility::b2s_coords(pos);
			//const auto &radius = cs.getRadius();
			//sfmlpos.x -= radius;
			//sfmlpos.y -= radius;
			cs.setPosition(sfmlpos);
			window.draw(cs);
		}
#if 0
		{
			static int fps;
			fps++;
			static auto starttime = std::chrono::high_resolution_clock::now();
			const auto measure_time = std::chrono::milliseconds(500);
			if (std::chrono::high_resolution_clock::now() - starttime < measure_time){
				starttime += measure_time;
				text.setString(std::to_string(fps*2));
				std::cout << fps * 2;
				fps = 0;
			}
			window.draw(text);
		}
#endif
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
		window.display();
	}
	System::clear<Components::PhysicalCircle>();
	Physical::world = nullptr;
}
