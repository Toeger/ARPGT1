#ifndef CAMERA_H
#define CAMERA_H

#include "window.h"
#include "Physics/physics_utility.h"

#include <array>

struct Camera
{
	Camera(Window &window);
	void set_position(float x, float y);
	std::array<float, 3> get_position() const;
	void look_at(float x, float y, float z);
	void set_aspect_ratio(float aspect_ratio);
	void turn_clockwise();
	void turn_counterclockwise();
	void zoom_in();
	void zoom_out();
private:
	//static constexpr Physical::Direction turn_speed{10.f, 1.f}; //this really should compile -.-
	static const Physical::Direction turn_speed;
	Physical::Direction direction;
	float camera_height = 100;
	static constexpr float zoom_speed = .9f;
	irr::scene::ICameraSceneNode *camera;
	irr::scene::ILightSceneNode *light;
};

#endif // CAMERA_H
