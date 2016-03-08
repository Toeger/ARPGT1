#ifndef CAMERA_H
#define CAMERA_H

#include "window.h"

#include <array>

struct Camera
{
	Camera(Window &window);
	void set_position(float x, float y, float z);
	std::array<float, 3> get_position() const;
	void look_at(float x, float y, float z);
private:
	irr::scene::ICameraSceneNode *camera;
};

#endif // CAMERA_H
