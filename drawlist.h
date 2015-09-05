#ifndef DRAWLIST_H
#define DRAWLIST_H

#include <SFML/Graphics.hpp>

namespace Drawlist{
    void add(sf::Shape &s);
    void remove(sf::Shape &s);
    void drawAll(sf::RenderWindow &r);
}

#endif // DRAWLIST_H
