#ifndef MAP_H
#define MAP_H

#include "graphics/perlinnoise.h"
#include "physics/shapes.h"

#include <array>
#include <bitset>

struct Map {
	//the map that the player is on, has nothing to do with std::map
	Map(int width, int height);
	//check if the given shape collides with something on the map
	bool collides(const Physical::Circle &circle, const Physical::Transformator &transformator);
	bool get(int x, int y) const;
	static Map *current_map;
	int get_block_size() const;
	int get_width() const;
	int get_height() const;
	std::pair<float, float> to_world_coords(const Physical::Vector &vec);

	private:
	int block_size = 100;
	std::vector<bool> create_map(int width, int height);
	int width;
	std::vector<bool> map; //bitmap of which tiles are blocked
};

#endif // MAP_H
