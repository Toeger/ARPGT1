#ifndef MAP_H
#define MAP_H

#include "Graphics/perlinnoise.h"
#include "Physics/shapes.h"

#include <array>
#include <bitset>

struct Map
{
	//the map that the player is on, has nothing to do with std::map
	Map(std::size_t width, std::size_t height);
	//check if the given shape collides with something on the map
	bool collides(const Physical::Circle &circle, const Physical::Transformator &transformator);
	int block_size = 100;
	bool get(int x, int y);
	static Map *current_map;
private:
	std::vector<bool> create_map(std::size_t width, std::size_t height);
	int width;
	std::vector<bool> map; //bitmap of which tiles are blocked
};

#endif // MAP_H
