#ifndef WINDOW_H
#define WINDOW_H

#include "ecs/utility.h"
#include "input_handler.h"
#include "physics/physics_utility.h"

#include <string_view>

struct Camera;

namespace irr {
	namespace scene {
		class IAnimatedMeshSceneNode;
		class ISceneManager;
		class ISceneNode;
	} // namespace scene
	namespace video {
		class ITexture;
		class IVideoDriver;
	} // namespace video
	class IrrlichtDevice;
} // namespace irr

struct Window {
	static Window get_dummy();
	Window(Input_handler &input_handler, int width = 800, int height = 600, std::string_view title = "");
	Window(Window &&) = default;
	Window(const Window &) = delete;
	Window &operator=(Window &&) = default;
	Window &operator=(const Window &) = delete;
	~Window();
	bool update(Camera &camera);
	static Window *current_window;
	irr::scene::IAnimatedMeshSceneNode *add_model(std::string_view path, Physical::Vector position = {}, Physical::Direction direction = {}, float size = 1.f,
												  irr::scene::ISceneNode *parent = nullptr);
	irr::video::ITexture *get_texture(std::string_view path) const;

	private:
	Utility::Move_only_pointer<irr::IrrlichtDevice> render_device;
	Utility::Move_only_pointer<irr::video::IVideoDriver> video_driver;
	Utility::Move_only_pointer<irr::scene::ISceneManager> scene_manager;
	friend struct Camera;
	friend struct Terrain;
	Window();
};

#endif // WINDOW_H
