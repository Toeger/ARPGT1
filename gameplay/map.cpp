#include "map.h"
#include "graphics/perlinnoise.h"
#include "physics/collision.h"
#include "physics/rect.h"
#include "utility/casts.h"

Map *Map::current_map;

Map::Map(int width, int height)
	: width(width)
	, map(create_map(width, height)) {}

bool Map::collides(const Physical::Circle &circle, const Physical::Transformator &transformator) {
	auto &pos = transformator.vector;
	const int startx = (pos.x - circle.radius) / block_size;
	const int endx = (pos.x + circle.radius) / block_size + 1;
	const int starty = (pos.y - circle.radius) / block_size;
	const int endy = (pos.y + circle.radius) / block_size + 1;
	for (int y = starty; y < endy; y++) {
		for (int x = startx; x < endx; x++) {
			if (get(x, y) && Physical::collides(circle, transformator, Physical::Rect(block_size, block_size),
												Physical::Transformator(Physical::Vector(x * block_size, y * block_size)))) {
				return true;
			}
		}
	}
	return false;
}

bool Map::get(int x, int y) const {
	if (x < 0 || y < 0 || x > width || y >= make_signed(map.size()) / width) {
		return true; //outside the map return true to make everything outside the map block the character
	}
	return map[x + width * y];
}

int Map::get_block_size() const {
	return block_size;
}

int Map::get_width() const {
	return width;
}

int Map::get_height() const {
	return static_cast<int>(map.size()) / width;
}

std::pair<float, float> Map::to_world_coords(const Physical::Vector &vec) {
	const auto block_size = Map::current_map->get_block_size();
	return {width - 1 - vec.x / block_size, vec.y / block_size};
}

std::vector<bool> Map::create_map(int width, int height) {
	std::vector<bool> retval(width * height);
	const auto min = 0.f;
	const auto max = 1.f;
	const auto separator = 0.47f;
	auto noise = get_perlin_noise(width, height, 40, min, max, 20);
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			retval[x + width * y] = noise[x + width * y] < separator;
		}
	}
	return retval;
}
