#ifndef TERRAIN_H
#define TERRAIN_H

#include <irrlicht/irrlicht.h>
#include "window.h"

struct Terrain
{
	Terrain(Window &window);
private:
	irr::scene::ITerrainSceneNode *terrain;
};

#endif // TERRAIN_H
