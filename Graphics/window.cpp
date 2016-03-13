#include "Utility/converter.h"
#include "window.h"
#include "camera.h"

#include <cassert>
#include <irrlicht/irrlicht.h>

Window::Window(Input_handler &input_handler, int width, int height, const std::string &title)
{
	//setup irrlicht graphics
	render_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(width, height), 32, false, false, true, &input_handler);
	assert(render_device);
	render_device->setWindowCaption(Converter::str_to_wstring(title).c_str());
	video_driver = render_device->getVideoDriver();
	scene_manager = render_device->getSceneManager();
}

Window::~Window()
{
	render_device->drop();
}

bool Window::update(Camera &camera)
{
	if (!render_device->run())
		return false;
	auto &vp = video_driver->getViewPort();
	camera.set_aspect_ratio(vp.getWidth() * 1.f / vp.getHeight());
	video_driver->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
	scene_manager->drawAll();
	video_driver->endScene();
	return true;
}

irr::scene::IAnimatedMeshSceneNode *Window::add_model(const std::string &path)
{
	auto mesh = scene_manager->getMesh(path.c_str());
	assert(mesh);
	auto node = scene_manager->addAnimatedMeshSceneNode(mesh);
	assert(node);
	return node;
}

irr::video::ITexture *Window::get_texture(const std::string &path) const
{
	return video_driver->getTexture(path.c_str());
}
