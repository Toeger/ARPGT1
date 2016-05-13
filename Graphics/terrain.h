#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>

#include "GamePlay/map.h"
#include "window.h"

namespace irr {
	namespace scene {
		class ITerrainSceneNode;
	}
}

struct Terrain {
	Terrain(Window &window, const Map &map, const std::string &texture);
	Terrain(Window &window, const std::string &height_bitmap, const std::string &texture_1, const std::string &texture_2);
	~Terrain();
	Terrain(const Terrain &) = delete;
	Terrain &operator=(const Terrain &) = delete;
	void set_position(float x, float y, float z);

	private:
	void create_terrain(Window &window, const std::string &hight_bitmap, const std::string &texture_1, const std::string &texture_2);
	irr::scene::ITerrainSceneNode *terrain;
};

#endif // TERRAIN_H
