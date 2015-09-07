#ifndef PHYSICALWALL_H
#define PHYSICALWALL_H

#include <Box2D/Box2D.h>

class PhysicalWall
{
public:
    PhysicalWall(b2World &world, b2Vec2 center_position, b2Vec2 size);
    ~PhysicalWall();
private:
    b2Body *body;
};

#endif // PHYSICALWALL_H
