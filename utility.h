#ifndef UTILITY
#define UTILITY

#include <cmath>
#include <limits>
#include <SFML/Graphics.hpp>

namespace Utility {
inline void normalize(float &a, float &b){
    auto length = sqrt(a * a + b * b);
    if (length < std::numeric_limits<float>::epsilon())
        return;
    a /= length;
    b /= length;
}
inline void rotate(float &a, float &b, float angle){
    auto a2 = a * cos(angle) - b * sin(angle);
    b = a * sin(angle) + b * cos(angle);
    a = a2;
}
}

#endif // UTILITY
