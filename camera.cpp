#include "camera.h"

Camera::Camera(sf::RenderWindow *window) :
	window(window)
{
	view.setCenter(400, 300);
	view.setSize(800, 600);
	if (window)
		window->setView(view);
}

void Camera::set_window(sf::RenderWindow *window)
{
	this->window = window;
	if (window)
		window->setView(view);
}

void Camera::set_position(const Physical::Vector &pos)
{
	view.setCenter(pos.x, -pos.y);
	//view.move(x, y);
	//view.setRotation(x);
	window->setView(view); //required even though the documentation says otherwise
}

void Camera::set_rotation(const Physical::Direction &rot)
{
	set_rotation(rot.to_radians());
}

void Camera::set_rotation(float angle)
{
	view.setRotation(angle);
}

void Camera::face(const Physical::Vector &pos)
{
	view.setRotation(atan2(pos.y, pos.x) * 180 / M_PI + 90);
}

void Camera::rotate(float turningangle)
{
	view.rotate(turningangle);
}

void Camera::set_size(int x, int y)
{
	view.setSize({static_cast<float>(x), static_cast<float>(y)});
}

float Camera::get_rotation() const
{
	return view.getRotation();
}

void Camera::set_zoom(float z)
{
	view.zoom(z);
}

sf::Vector2f Camera::get_screen_size() const
{
	return view.getSize();
}

Camera::~Camera()
{
	if (window)
		window->setView(window->getDefaultView());
}
