#ifndef COMMON_GRAPHICS_COMPONENTS_H
#define COMMON_GRAPHICS_COMPONENTS_H

#include "physics/physics_utility.h"
#include "window.h"

#include <experimental/string_view>

using string_view = std::experimental::string_view;

namespace Common_components {
	struct Animated_model {
		Animated_model(Window &window, string_view path, string_view texture);
		Animated_model(Window &window, string_view path);
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
		irr::scene::IAnimatedMeshSceneNode *node = nullptr;
	};
};

#endif // COMMON_GRAPHICS_COMPONENTS_H
