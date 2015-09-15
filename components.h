#ifndef COMPONENTS
#define COMPONENTS

#include <SFML/Graphics.hpp>

namespace Components{
struct Position{
    float x, y;
};

struct NormalizedDirection{
    float x, y;
};

using Velocity = float;
using sf::CircleShape;
using sf::RectangleShape;
}

#endif // COMPONENTS

