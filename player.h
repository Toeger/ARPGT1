#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "camera.h"
#include "weapon.h"
#include "entity.h"
#include "Physics/physics_utility.h"

class Player : public Entity
{
public:
	Player(sf::RenderWindow *window = nullptr);
	void set_window(sf::RenderWindow *window);
	Camera camera;
	static Player player;
	Physical::Direction move_direction;
	float speed = 1;
private:
};

#endif // PLAYER_H
