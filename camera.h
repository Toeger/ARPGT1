#ifndef CAMERA_H
#define CAMERA_H

#include "Physics/physics_utility.h"

#include <SFML/Graphics.hpp>

class Camera
{
public:
	Camera(sf::RenderWindow *window = nullptr);
	void set_window(sf::RenderWindow *window);
	void set_position(const Physical::Vector &pos);
	void set_rotation(const Physical::Direction &rot);
	void set_rotation(float angle);
	void rotate(float turningangle);
	void set_size(int x, int y);
	float get_rotation() const;
	void set_zoom(float z);
	sf::Vector2f get_screen_size() const;
	~Camera();
private:
	sf::RenderWindow *window;
	sf::View view;
};

#endif // CAMERA_H
