#ifndef PHYSICALWALL_H
#define PHYSICALWALL_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class PhysicalWall
{
public:
    PhysicalWall(
            b2World &world,
            b2Vec2 center_position,
            b2Vec2 size,
            bool visible = false,
            sf::Color color = sf::Color::Black);
    ~PhysicalWall();
private:
    b2Body *body;
    sf::RectangleShape debugGraphics;
};

#endif // PHYSICALWALL_H
