#include "Graphics/perlinnoise.h"
#include "map.h"
#include "Physics/collision.h"
#include "Physics/rect.h"

Map::Map(std::size_t width, std::size_t height)
	: width(width)
	, map(create_map(width, height))
{}

bool Map::collides(const Physical::Circle &circle, const Physical::Transformator &transformator)
{
	auto &pos = transformator.vector;
	auto startx = (pos.x - circle.radius) / block_size;
	auto endx = (pos.x + circle.radius) / block_size + 1;
	auto starty = (pos.y - circle.radius) / block_size;
	auto endy = (pos.y + circle.radius) / block_size + 1;
	//XXX FIME: just because the outside of the map could potentially collide with the shape doesn't mean it will. need to do this better
	if (startx < 0 || endx >= width || starty < 0 || endy >= map.size() / width) //out of range of the map
		return true;
	for (int x = startx; x < endx; x++){
		for (int y = starty; y < endy; y++){
			if (Physical::collides(circle, transformator, Physical::Rect(block_size, block_size),
						 Physical::Transformator(Physical::Vector(x * block_size, y * block_size))))
				return true;
		}
	}
	return false;
}

bool Map::get(int x, int y)
{
	if (x < 0 || y < 0 || x > width || x + width * y > static_cast<int>(map.size()))
		return true; //outside the map return true to make everything outside the map block the character
	return map[x + width * y];
}

std::vector<bool> Map::create_map(std::size_t width, std::size_t height)
{
	std::vector<bool> retval(width * height);
	const auto min = 0.f;
	const auto max = 1.f;
	const auto separator = 0.47f;
	auto noise = get_perlin_noise(width, height, 40, min, max, 20);
	for (std::size_t y = 0; y < height; y++){
		for (std::size_t x = 0; x < width; x++){
			retval[x + width * y] = noise[x + width * y] < separator;
		}
	}
	return retval;
}
