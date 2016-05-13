#ifndef COMMON_GRAPHICS_COMPONENTS_H
#define COMMON_GRAPHICS_COMPONENTS_H

#include "Physics/physics_utility.h"
#include "window.h"

#include <string>

namespace Common_components {
	struct Animated_model {
		Animated_model(Window &window, const std::string &path, const std::string &texture);
		Animated_model(Window &window, const std::string &path);
		Animated_model(Animated_model &&other);
		Animated_model(const Animated_model &) = delete;
		Animated_model &operator=(const Animated_model &) = delete;
		Animated_model &operator=(Animated_model &&other);
		~Animated_model();
		void set_position(const std::pair<float, float> &xy_coords);
		void set_position(float x, float y, float z);
		void set_position_direction(float x, float y, float direction);
		void look_at(float x, float y);
		void set_rotation(float angle);

		private:
		irr::scene::IAnimatedMeshSceneNode *node;
	};
};

#endif // COMMON_GRAPHICS_COMPONENTS_H
