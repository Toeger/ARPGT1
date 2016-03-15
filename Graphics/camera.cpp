#include "camera.h"

#include <irrlicht/irrlicht.h>

const Physical::Direction Camera::turn_speed{10, 1};

Camera::Camera(Window &window)
{
	//camera = window.scene_manager->addCameraSceneNodeFPS(0,100.0f,1.2f);
	camera = window.scene_manager->addCameraSceneNode();
	camera->setFarValue(1000000.0f);
	light = window.scene_manager->addLightSceneNode(nullptr, {20, 100, 20}, {1, 1, 1, .5f}, 100);
	light->setRotation({0, 270, 90});
}

void Camera::set_position(float x, float y)
{
	light->setPosition({x, 100, y});
	camera->setPosition({x + direction.get_x() * camera_height / 2, camera_height, y + direction.get_y() * camera_height / 2});
}

std::array<float, 3> Camera::get_position() const
{
	auto pos = camera->getPosition();
	return {{pos.X, pos.Y, pos.Z}};
}

const Physical::Direction &Camera::get_direction() const
{
	return direction;
}

void Camera::look_at(float x, float y, float z)
{
	camera->setTarget({x, y, z});
}

void Camera::set_aspect_ratio(float aspect_ratio)
{
	static float last_aspect_ratio = 1.f;
	if (aspect_ratio - last_aspect_ratio < 0.001f)
		return;
	last_aspect_ratio = aspect_ratio;
	camera->setAspectRatio(aspect_ratio);
}

void Camera::turn_clockwise()
{
	direction += turn_speed;
}

void Camera::turn_counterclockwise()
{
	direction -= turn_speed;
}

void Camera::set_light_position(float x, float y, float z)
{
	light->setPosition({x, y, z});
}

std::array<float, 3> Camera::get_light_position() const
{
	auto pos = light->getPosition();
	return {{pos.X, pos.Y, pos.Z}};
}

void Camera::zoom_in()
{
	camera_height *= zoom_speed;
}

void Camera::zoom_out()
{
	camera_height /= zoom_speed;
}
