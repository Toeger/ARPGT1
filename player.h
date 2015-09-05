#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();
    void logicalUpdate();
    sf::CircleShape character;
};

#endif // PLAYER_H
