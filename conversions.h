#ifndef CONVERSIONS
#define CONVERSIONS

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

namespace Conversions {
inline sf::Vector2f b2ToSfVector(const b2Vec2 bv){
    return {bv.x, bv.y};
}
inline sf::Vector2f boxPosToSfCirclePos(const b2Vec2 bv, float diameter){
    return {bv.x - diameter, bv.y - diameter};
}
}

#endif // CONVERSIONS

