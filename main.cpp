#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <Box2D/Box2D.h>
#include <memory>
#include <tuple>
#include <initializer_list>
#include <array>

#include "drawlist.h"
#include "player.h"
#include "updatelist.h"
#include "logicalobject.h"
#include "practicedummy.h"
#include "physicalwall.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "ARPGT1");
    auto &now =  std::chrono::high_resolution_clock::now;
    auto lastUpdate = now();
    const auto lfps = 30;
    using namespace std::chrono_literals;
    b2World world({0, 0});

    const float thickness = 1;
    std::array<PhysicalWall, 4> boundingbox
    {
        PhysicalWall{world, {400, 600}, {800, thickness}},
        PhysicalWall{world, {400, 0}, {800, thickness}},
        PhysicalWall{world, {0, 300}, {thickness, 600}},
        PhysicalWall{world, {800, 300}, {thickness, 600}},
    };

    Player p{world};
    PracticeDummy pd{world};

    while (window.isOpen()){
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //UpdateList::updateAll();
        if (now() - lastUpdate > 1s / lfps){
            world.Step(1, 100, 100);
            LogicalObject::update();
            lastUpdate += 1s / lfps;
        }
        window.clear(sf::Color::Black);
        Drawlist::drawAll(window);
        window.display();
    }
}
