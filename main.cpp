#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <Box2D/Box2D.h>
#include <memory>
#include <tuple>
#include <initializer_list>

#include "drawlist.h"
#include "player.h"
#include "updatelist.h"
#include "logicalobject.h"
#include "practicedummy.h"

int main()
{
    sf::String s = "ARPGT1";
    sf::RenderWindow window(sf::VideoMode(800, 600), s);
    auto &now =  std::chrono::high_resolution_clock::now;
    auto lastUpdate = now();
    const auto lfps = 30;
    using namespace std::chrono_literals;
    b2World world({0, 0});
    b2BodyDef bodyDef;
    struct WallDimensions{
        float centerX;
        float centerY;
        float halfWidth;
        float halfHeight;
    };

    for (auto &dims : std::initializer_list<WallDimensions>
        {
            {400, 600, 400, 1},
            {400, 0, 400, 1},
            {0, 300, 1, 300},
            {800, 300, 1, 300},
        }
    ){
        bodyDef.position.Set(dims.centerX, dims.centerY);
        b2Body *body = world.CreateBody(&bodyDef);
        //TODO: world.DestroyBody(groundBody);
        b2PolygonShape box;
        box.SetAsBox(dims.halfWidth, dims.halfHeight);
        body->CreateFixture(&box, 0.0f);
    }

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
            world.Step(10, 10, 10);
            LogicalObject::update();
            lastUpdate += 1s / lfps;
        }
        window.clear(sf::Color::Black);
        Drawlist::drawAll(window);
        window.display();
    }
}
