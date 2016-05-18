#include "common_graphics_components.h"
#include "utility/casts.h"

#include <cmath>
#include <irrlicht/irrlicht.h>

Common_components::Animated_model::Animated_model(Window &window, const std::string &path, const std::string &texture)
	: Common_components::Animated_model(window, path) {
	node->setMaterialTexture(0, window.get_texture(texture.c_str()));
}

Common_components::Animated_model::Animated_model(Window &window, const std::string &path) {
	node = window.add_model(path);
	//node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	node->addShadowVolumeSceneNode();
	node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
}

Common_components::Animated_model::Animated_model(Common_components::Animated_model &&other) {
	std::swap(node, other.node);
}

Common_components::Animated_model &Common_components::Animated_model::operator=(Common_components::Animated_model &&other) {
	std::swap(node, other.node);
	return *this;
}

Common_components::Animated_model::~Animated_model() {
	if (node) {
		node->remove();
	}
}

void Common_components::Animated_model::set_position(const std::pair<float, float> &xy_coords) {
	set_position(xy_coords.first, 0, xy_coords.second);
}

void Common_components::Animated_model::set_position(float x, float y, float z) {
	node->setPosition({x, y, z});
}

void Common_components::Animated_model::set_position_direction(float x, float y, float direction) {
	set_position(x, 0, y);
	set_rotation(direction);
}

void Common_components::Animated_model::look_at(float x, float y) {
	const auto &pos = node->getPosition();
	const auto dy = y - pos.Z;
	const auto dx = x - pos.X;
	const auto angle = std::atan2(-dy, dx) * 180 / precision_cast(M_PI);
	node->setRotation({0, angle, 0});
}

void Common_components::Animated_model::set_rotation(float angle) {
	node->setRotation({0, angle, 0});
}
