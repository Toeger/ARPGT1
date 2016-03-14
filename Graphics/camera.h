#ifndef CAMERA_H
#define CAMERA_H

#include "window.h"

#include <array>

struct Camera
{
	Camera(Window &window);
	void set_position(float x, float y);
	std::array<float, 3> get_position() const;
	void look_at(float x, float y, float z);
	void set_aspect_ratio(float aspect_ratio);
	float camera_height = 100;
private:
	irr::scene::ICameraSceneNode *camera;
	irr::scene::ILightSceneNode *light;
};

#endif // CAMERA_H
