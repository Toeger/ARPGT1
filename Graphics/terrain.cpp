#include "terrain.h"

Terrain::Terrain(Window &window, const std::string &hight_bitmap, const std::string &texture)
{
	terrain = window.scene_manager->addTerrainSceneNode(
				hight_bitmap.c_str(),
				0,												//parent node
				-1,												//node id
				irr::core::vector3df(0.f, 0.f, 0.f),			//position
				irr::core::vector3df(0.f, 0.f, 0.f),			//rotation //comment says this is not yet implemented
				irr::core::vector3df(1.f, 15.f / 255.f, 1.f),	//scale
				irr::video::SColor(255, 255, 255, 255),			//vertexColor
				5,												//maxLOD //TODO: figure out what this actually does
				irr::scene::ETPS_9,								//patchSize //TODO: figure out what this actually does
				0												//smoothFactor //shouldn't need to smooth since perlin does smoothing just fine
				);
	terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0, window.video_driver->getTexture(hight_bitmap.c_str()));
	terrain->setMaterialTexture(1, window.video_driver->getTexture(texture.c_str()));
	terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
	terrain->scaleTexture(1.f, 50.f);
}

void Terrain::set_position(float x, float y, float z)
{
	terrain->setPosition({x, y, z});
}
