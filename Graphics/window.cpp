#include "Utility/converter.h"
#include "window.h"

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

bool Window::update()
{
	if (!render_device->run())
		return false;
	video_driver->beginScene(true, true, irr::video::SColor(255, 100, 101, 140));
	scene_manager->drawAll();
	video_driver->endScene();
	return true;
}
