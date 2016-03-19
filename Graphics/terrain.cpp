#include "ECS/utility.h"
#include "terrain.h"
#include "Utility/casts.h"

#include <irrlicht/irrlicht.h>

Terrain::Terrain(Window &window, const Map &map, const std::string &texture)
{
	auto height_map = window.video_driver->createImage(irr::video::ECOLOR_FORMAT::ECF_R8G8B8, {
														   make_unsigned(map.get_width()),
														   make_unsigned(map.get_height())});
	ON_SCOPE_EXIT(height_map->drop(););
	auto inverse_height_map = window.video_driver->createImage(irr::video::ECOLOR_FORMAT::ECF_R8G8B8, {
																   make_unsigned(map.get_width()),
																   make_unsigned(map.get_height())});
	ON_SCOPE_EXIT(inverse_height_map->drop(););
	for (int y = 0; y < map.get_height(); y++){
		for (int x = 0; x < map.get_width(); x++){
			unsigned char c = map.get(x, y) * 255;
			unsigned char ic = map.get(x, y) ? 0 : 255;
			height_map->setPixel(x, y, {0, c, c, c});
			inverse_height_map->setPixel(x, y, {0, ic, ic, ic});
		}
	}
	const std::string height_bitmap_name = "/tmp/height_map.bmp";
	const std::string inverse_height_map_name = "/tmp/inverse_height_map.png";
	window.video_driver->writeImageToFile(height_map, height_bitmap_name.c_str());
	window.video_driver->writeImageToFile(inverse_height_map, inverse_height_map_name.c_str());
	create_terrain(window, height_bitmap_name, inverse_height_map_name, texture);
}

Terrain::Terrain(Window &window, const std::string &height_bitmap, const std::string &texture_1, const std::string &texture_2)
{
	create_terrain(window, height_bitmap, texture_1, texture_2);
}

void Terrain::set_position(float x, float y, float z)
{
	terrain->setPosition({x, y, z});
}

void Terrain::create_terrain(Window &window, const std::string &hight_bitmap, const std::string &texture_1, const std::string &texture_2)
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
	//terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0, window.video_driver->getTexture(texture_1.c_str()));
	terrain->setMaterialTexture(1, window.video_driver->getTexture(texture_2.c_str()));
	terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
	terrain->scaleTexture(1.f, 50.f);
	window.scene_manager->getMeshManipulator()->makePlanarTextureMapping(terrain->getMesh(), 0.004f);
	//terrain->addShadowVolumeSceneNode();
	terrain->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
}
