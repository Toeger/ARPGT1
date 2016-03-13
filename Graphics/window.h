#ifndef WINDOW_H
#define WINDOW_H

#include "input_handler.h"

#include <irrlicht/irrlicht.h>
#include <string>

struct Camera;

struct Window
{
	Window(Input_handler &input_handler, int width = 800, int height = 600, const std::string &title = "");
	~Window();
	bool update(Camera &camera);
	irr::scene::IAnimatedMeshSceneNode *add_model(const std::string &path);
	irr::video::ITexture *get_texture(const std::string &path) const;
//private:
	irr::IrrlichtDevice *render_device;
	irr::video::IVideoDriver *video_driver;
	irr::scene::ISceneManager *scene_manager;
	friend struct Camera;
	friend struct Terrain;
};

#endif // WINDOW_H
