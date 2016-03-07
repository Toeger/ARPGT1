#ifndef TERRAIN_H
#define TERRAIN_H

#include <irrlicht/irrlicht.h>
#include <string>

#include "window.h"

struct Terrain
{
	Terrain(Window &window, const std::string &hight_bitmap, const std::__cxx11::string &texture);
	void set_position(float x, float y, float z);
private:
	irr::scene::ITerrainSceneNode *terrain;
};

#endif // TERRAIN_H
