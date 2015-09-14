#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "camera.h"
#include "weapon.h"
#include "entity.h"

class Player : public Entity
{
public:
    Player(sf::RenderWindow *window);
    void logicalUpdate();
    float speed = 50;
    Camera camera;
    Weapon weapon;
private:
};

#endif // PLAYER_H
