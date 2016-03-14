#ifndef COMMON_GRAPHICS_COMPONENTS_H
#define COMMON_GRAPHICS_COMPONENTS_H

#include "window.h"

#include <string>

namespace Common_components
{
	struct Animated_model{
		Animated_model(Window &window, const std::string &path, const std::string &texture);
		~Animated_model();
		void set_position(float x, float y, float z);
		void look_at(float x, float y);
		irr::scene::IAnimatedMeshSceneNode *node;
	};
};

#endif // COMMON_GRAPHICS_COMPONENTS_H
