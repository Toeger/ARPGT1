#include "camera.h"

Camera::Camera(Window &window)
{
	//auto camera = scene_manager->addCameraSceneNodeFPS(0,100.0f,1.2f);
	camera = window.scene_manager->addCameraSceneNode();
	camera->setFarValue(100000.0f);
}

void Camera::set_position(float x, float y, float z)
{
	camera->setPosition({x, y, z});
}

void Camera::look_at(float x, float y, float z)
{
	camera->setTarget({x, y, z});
}