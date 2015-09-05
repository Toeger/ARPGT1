#include <iostream>
#include <SFML/Graphics.hpp>

#include "drawlist.h"
#include "player.h"
#include "updatelist.h"
#include "logicalobject.h"

int main()
{
    Player p;
    sf::RenderWindow window(sf::VideoMode(800, 600), "ARPGT1");
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
        LogicalObject::update();
        window.clear(sf::Color::Black);
        Drawlist::drawAll(window);
        window.display();
    }
}
