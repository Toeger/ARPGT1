#include "window.h"
#include "camera.h"
#include "utility/asserts.h"
#include "utility/converter.h"

#include <irrlicht/irrlicht.h>

Window *Window::current_window;

Window Window::get_dummy() {
	return {};
}

Window::Window(Input_handler &input_handler, int width, int height, std::string_view title) {
	//setup irrlicht graphics
	render_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(width, height), 32, false, true, true, &input_handler);
	assert_fast(render_device);
	render_device->setWindowCaption(Converter::str_to_wstring(title).c_str());
	video_driver = render_device->getVideoDriver();
	scene_manager = render_device->getSceneManager();
	scene_manager->setShadowColor({150, 0, 0, 0});
}

Window::~Window() {
	if (render_device) {
		render_device->drop();
	}
}

bool Window::update(Camera &camera) {
	if (!render_device->run()) {
		return false;
	}
	auto &vp = video_driver->getViewPort();
	camera.set_aspect_ratio(vp.getWidth() * 1.f / vp.getHeight());
	video_driver->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
	scene_manager->drawAll();
	video_driver->endScene();
	return true;
}

irr::scene::IAnimatedMeshSceneNode *Window::add_model(std::string_view path, Physical::Vector position, Physical::Direction direction, float scale,
													  irr::scene::ISceneNode *parent) {
	auto mesh = scene_manager->getMesh(path.data());
	assert_fast(mesh);
	auto node = scene_manager->addAnimatedMeshSceneNode(mesh, parent, -1, {position.x, 0, position.y});
	node->setRotation({0, direction.to_degrees(), 0});
	node->setScale({scale, scale, scale});
	assert_fast(node);
	return node;
}

irr::video::ITexture *Window::get_texture(std::string_view path) const {
	return video_driver->getTexture(path.data());
}

Window::Window() {
	//setup irrlicht graphics
	render_device = irr::createDevice(irr::video::EDT_NULL);
	assert_fast(render_device);
	video_driver = render_device->getVideoDriver();
	scene_manager = render_device->getSceneManager();
}
