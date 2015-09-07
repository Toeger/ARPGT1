#include "camera.h"

Camera::Camera(sf::RenderWindow *window) :
    window(window)
{
    view.setCenter(400, 300);
    view.setSize(800, 600);
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


const float turningangle = .3f;
void Camera::rotate_left()
{
    view.rotate(-turningangle);
}

void Camera::rotate_right()
{
    view.rotate(turningangle);
}

float Camera::get_rotation() const
{
    return view.getRotation();
}

Camera::~Camera()
{
    window->setView(window->getDefaultView());
}
