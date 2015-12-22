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

void Camera::set_position(float x, float y)
{
	view.setCenter(x, y);
	//view.move(x, y);
	//view.setRotation(x);
	window->setView(view); //required even though the documentation says otherwise
}

void Camera::set_rotation(float angle)
{
	view.setRotation(angle);
}

void Camera::face(float x, float y)
{
	view.setRotation(atan2(y, x) * 180 / M_PI + 90);
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

#include <iostream>
void Camera::set_zoom(float z)
{
	std::cout << z << std::endl;
	view.zoom(z);
}

Camera::~Camera()
{
	if (window)
		window->setView(window->getDefaultView());
}
