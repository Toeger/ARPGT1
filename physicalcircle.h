#ifndef PHYSICALCIRCLE_H
#define PHYSICALCIRCLE_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class PhysicalCircle
{
public:
    PhysicalCircle(b2World &world, b2Vec2 position, float radius, float friction = 1.f);
    b2Vec2 get_position() const;
    void apply_position(sf::CircleShape &circle) const;
    void set_velocity(b2Vec2 velocity);
    b2World *get_world();
    ~PhysicalCircle();
//private:
    b2Body *body;
};

#endif // PHYSICALCIRCLE_H
