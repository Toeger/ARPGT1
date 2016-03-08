#include "camera.h"

Camera::Camera(Window &window)
{
	//camera = window.scene_manager->addCameraSceneNodeFPS(0,100.0f,1.2f);
	camera = window.scene_manager->addCameraSceneNode();
	camera->setFarValue(100000.0f);
}

void Camera::set_position(float x, float y, float z)
{
	camera->setPosition({x, y, z});
}

std::array<float, 3> Camera::get_position() const
{
	auto pos = camera->getPosition();
	return {{pos.X, pos.Y, pos.Z}};
}

void Camera::look_at(float x, float y, float z)
{
	camera->setTarget({x, y, z});
}
