#ifndef TERRAIN_H
#define TERRAIN_H

#include "gameplay/map.h"
#include "window.h"

#include <experimental/string_view>

using string_view = std::experimental::string_view;

namespace irr {
	namespace scene {
		class ITerrainSceneNode;
	}
}

struct Terrain {
	Terrain(Window &window, const Map &map, string_view texture);
	Terrain(Window &window, string_view height_bitmap, string_view texture_1, string_view texture_2);
	~Terrain();
	Terrain(const Terrain &) = delete;
	Terrain &operator=(const Terrain &) = delete;
	void set_position(float x, float y, float z);

	private:
	void create_terrain(Window &window, string_view hight_bitmap, string_view texture_1, string_view texture_2);
	irr::scene::ITerrainSceneNode *terrain;
};

#endif // TERRAIN_H
