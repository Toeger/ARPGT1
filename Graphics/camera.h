#ifndef CAMERA_H
#define CAMERA_H

#include "window.h"

struct Camera
{
	Camera(Window &window);
	void set_position(float x, float y, float z);
	void look_at(float x, float y, float z);
private:
	irr::scene::ICameraSceneNode *camera;
};

#endif // CAMERA_H
