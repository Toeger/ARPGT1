#ifndef COMPONENTS
#define COMPONENTS

#include <SFML/Graphics.hpp>

namespace Components{
struct Position{
    double x, y;
};

struct Direction{
    double x, y;
};

using Velocity = double;
using sf::CircleShape;
using sf::RectangleShape;
}

#endif // COMPONENTS

