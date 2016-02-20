#include "perlin_test.h"
#include "Graphics/perlinnoise.h"

#include <SFML/Graphics.hpp>

#include <cassert>

void test_perlin()
{
	//not sure how to automatically test perlin noise, so just dump an image for now and manually check it
	const int width = 1000;
	const int height = 1000;
	const int min = 0;
	const int max = 255;
	auto noise = get_perlin_noise<float, width, height, 1>(0, 255);
	sf::Image image;
	image.create(width, height);
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			auto &value = noise[x][y];
			assert(value >= min);
			assert(value <= max);
			image.setPixel(x, y, sf::Color(value, value, value));
		}
	}
	image.saveToFile("/tmp/perlin.png");
}
