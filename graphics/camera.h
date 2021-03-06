#ifndef CAMERA_H
#define CAMERA_H

#include "physics/physics_utility.h"
#include "window.h"

#include <array>

namespace irr {
	namespace scene {
		class ICameraSceneNode;
		class ILightSceneNode;
	} // namespace scene
} // namespace irr

struct Camera {
	Camera(Window &window);
	Camera(const Camera &) = delete;
	Camera &operator=(const Camera &) = delete;
	void set_position(float x, float y);
	std::array<float, 3> get_position() const;
	const Physical::Direction &get_direction() const;
	void look_at(float x, float y, float z);
	void set_aspect_ratio(float aspect_ratio);
	void turn_clockwise();
	void turn_counterclockwise();
	std::array<float, 3> get_light_position() const;
	void set_light_position(float x, float y, float z);
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
