#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <memory>
#include <tuple>
#include <initializer_list>
#include <array>

#include "player.h"
#include "logicalobject.h"
#include "practicedummy.h"
#include "entity.h"
#include "components.h"

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
    PracticeDummy pd;

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
            //for (auto &e : System::get_components<Components::Position, Components::Velocity, Components::Direction>())
        }
        window.clear(sf::Color::Black);
        //rendering system
        for (auto &c : System::get_components<Components::CircleShape>()){
            window.draw(c);
        }
        window.display();
    }
}
