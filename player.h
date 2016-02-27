#ifndef PLAYER_H
#define PLAYER_H

#include "camera.h"
#include "ECS/entity.h"
#include "Physics/physics_utility.h"

#include <SFML/Graphics.hpp>

class Player : public ECS::Entity
{
public:
	Player(sf::RenderWindow *window = nullptr);
	void set_window(sf::RenderWindow *window);
	Physical::Vector move_offset;
	Physical::Direction turn_speed = Physical::Direction::from_degrees(4);
	Physical::Direction turn_offset;
	void center_camera();
	void turn(float angle);
	static Player player;
	Camera camera;
private:
};

#endif // PLAYER_H
