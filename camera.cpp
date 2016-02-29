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

Physical::AABB Camera::get_visual_aabb() const
{
	Physical::AABB retval;
	//TODO: do some view calculation to figure out what the camera can see
	auto center = view.getCenter();
	auto size = view.getSize();
	retval.left = center.x - size.x;
	retval.right = center.x + size.x;
	retval.bottom = center.y - size.y;
	retval.top = center.y + size.y;
	return retval;
}

Camera::~Camera()
{
	if (window)
		window->setView(window->getDefaultView());
}
