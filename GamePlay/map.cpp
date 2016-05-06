#include "map.h"
#include "Graphics/perlinnoise.h"
#include "Physics/collision.h"
#include "Physics/rect.h"
#include "Utility/casts.h"

Map *Map::current_map;

Map::Map(std::size_t width, std::size_t height)
	: width(width)
	, map(create_map(width, height)) {}

bool Map::collides(const Physical::Circle &circle, const Physical::Transformator &transformator) {
	auto &pos = transformator.vector;
	auto startx = (pos.x - circle.radius) / block_size;
	auto endx = (pos.x + circle.radius) / block_size + 1;
	auto starty = (pos.y - circle.radius) / block_size;
	auto endy = (pos.y + circle.radius) / block_size + 1;
	for (int y = starty; y < endy; y++) {
		for (int x = startx; x < endx; x++) {
			if (get(x, y) && Physical::collides(circle, transformator, Physical::Rect(block_size, block_size),
												Physical::Transformator(Physical::Vector(x * block_size, y * block_size))))
				return true;
		}
	}
	return false;
}

bool Map::get(int x, int y) const {
	if (x < 0 || y < 0 || x > width || y >= make_signed(map.size()) / width)
		return true; //outside the map return true to make everything outside the map block the character
	return map[x + width * y];
}

int Map::get_block_size() const {
	return block_size;
}

int Map::get_width() const {
	return width;
}

int Map::get_height() const {
	return make_signed(map.size()) / width;
}

std::pair<float, float> Map::to_world_coords(const Physical::Vector &vec)
{
	const auto block_size = Map::current_map->get_block_size();
	return {width - 1 - vec.x / block_size, vec.y / block_size};
}

std::vector<bool> Map::create_map(std::size_t width, std::size_t height) {
	std::vector<bool> retval(width * height);
	const auto min = 0.f;
	const auto max = 1.f;
	const auto separator = 0.47f;
	auto noise = get_perlin_noise(width, height, 40, min, max, 20);
	for (std::size_t x = 0; x < width; x++) {
		for (std::size_t y = 0; y < height; y++) {
			retval[x + width * y] = noise[x + width * y] < separator;
		}
	}
	return retval;
}
