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
    Camera camera;
private:
};

#endif // PLAYER_H
