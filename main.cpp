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
#include "components.h"
#include "utility.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "ARPGT1");
    auto &now =  std::chrono::high_resolution_clock::now;
    auto last_update_timepoint = now();
    const auto lfps = 30;
    const auto logical_frame_duration = std::chrono::milliseconds(1000) / lfps;
    using namespace std::chrono_literals;

    window.setVerticalSyncEnabled(true);

    Player p(&window);
    p.camera.rotate(0);
    p.add(Components::Velocity{3});
    p.add(Components::NormalizedDirection{});
    p.add(Components::Position{20, 2});
    p.add(Components::CircleShape(40));
    PracticeDummy pd;
    pd.add(Components::Velocity{3});
    pd.add(Components::NormalizedDirection{});
    pd.add(Components::Position{20, 2});
    pd.add(Components::CircleShape(40));
    Entity e;
    e.add(Components::Velocity{3});
    e.add(Components::NormalizedDirection{});
    e.add(Components::Position{1, 2});

    while (window.isOpen()){
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                p.camera.set_size(event.size.width, event.size.height);
                //window.setSize({event.size.width, event.size.height});
        }
        //UpdateList::updateAll();
        while (now() - last_update_timepoint > logical_frame_duration)
        {
            last_update_timepoint += logical_frame_duration;
            //setting player direction and velocity
            {
                auto &vel = *p.get<Components::Velocity>();
                auto &dir = *p.get<Components::NormalizedDirection>();
                vel = 0;
                dir.x = 0;
                dir.y = 0;
                const auto camera_turning_speed = 5.f; //in degree
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
                    dir.y = -1;
                    vel = 10;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
                    dir.x = -1;
                    vel = 10;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                    dir.y = 1;
                    vel = 10;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                    dir.x = 1;
                    vel = 10;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
                    p.camera.rotate(-camera_turning_speed);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
                    p.camera.rotate(camera_turning_speed);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)){
                    const auto &dummy_pos = *pd.get<Components::Position>();
                    const auto &player_pos = *p.get<Components::Position>();
                    p.camera.set_rotation(atan2(dummy_pos.y - player_pos.y, dummy_pos.x - player_pos.x) * 180 / M_PI + 90);
                }
                Utility::normalize(dir.x, dir.y);
                Utility::rotate(dir.x, dir.y, p.camera.get_rotation() * M_PI / 180);
            }
            //resolve physics
            for (auto sit = System::begin<Components::Position, Components::Velocity, Components::NormalizedDirection>(); sit; sit.advance()){
                auto &vel = sit.get<Components::Velocity>();
                if (vel == 0) //TODO: double == comparison bad?
                    continue;
                auto &dir = sit.get<Components::NormalizedDirection>();
                auto &pos = sit.get<Components::Position>();
                pos.x += dir.x * vel;
                pos.y += dir.y * vel;
            }
        }
        window.clear(sf::Color::Black);
        //rendering system
        auto &pos = *p.get<Components::Position>();
        p.camera.set_position(pos.x, pos.y);
        for (auto sit = System::begin<Components::CircleShape, Components::Position>(); sit; sit.advance()){
            auto &pos = sit.get<Components::Position>();
            auto &c = sit.get<Components::CircleShape>();
            c.setPosition(pos.x - c.getRadius(), pos.y - c.getRadius());
            window.draw(c);
        }
        window.display();
    }
}
