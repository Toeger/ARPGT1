#ifndef UTILITY
#define UTILITY

#include <Box2D/Box2D.h>

namespace Utility {
inline void rotate(b2Vec2 &vec, float angle){
    angle *= M_PI / 180;
    auto x = (vec.x * cos(angle)) - (vec.y * sin(angle));
    auto y = (vec.y * cos(angle)) + (vec.x * sin(angle));
    vec.x = x;
    vec.y = y;
}

}

#endif // UTILITY

