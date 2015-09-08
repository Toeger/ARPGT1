#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "physicalcircle.h"
#include "camera.h"
#include "weapon.h"

class Player
{
public:
    Player(b2World &world, sf::RenderWindow *window);
    void logicalUpdate();
    sf::CircleShape character;
    float speed = 30;
    PhysicalCircle pc;
    Camera camera;
    Weapon weapon;
private:
    b2World *get_world();
};

#endif // PLAYER_H
