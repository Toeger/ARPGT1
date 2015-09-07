#ifndef SENSOR_H
#define SENSOR_H

#include <Box2D/Box2D.h>
#include <vector>

namespace Sensor
{
    std::vector<b2Body *> get_collisions(b2Body *body);
}

#endif // SENSOR_H
