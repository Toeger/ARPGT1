#include "terrain.h"

Terrain::Terrain(Window &window)
{
	terrain = window.scene_manager->addTerrainSceneNode(
				"Art/perlin_map.bmp",
				0,											//parent node
				-1,											//node id
				irr::core::vector3df(0.f, 0.f, 0.f),		//position
				irr::core::vector3df(0.f, 0.f, 0.f),		//rotation
				irr::core::vector3df(10.f, 1.f, 10.f),		//scale
				irr::video::SColor ( 255, 255, 255, 255 ),	//vertexColor
				5,											//maxLOD
				irr::scene::ETPS_9,							//patchSize
				0											//smoothFactor
				);
	terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0, window.video_driver->getTexture("Art/wood-rotten.png"));
	terrain->setMaterialTexture(1, window.video_driver->getTexture("Art/Parchment.jpg"));
	terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
	terrain->scaleTexture(10.f, 1.f);
}
