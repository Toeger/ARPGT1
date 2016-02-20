#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include "Utility/asserts.h"

#include <array>
#include <numeric>
#include <random>
#include <type_traits>
#include <algorithm>

static std::random_device rd;
static std::mt19937 mt(rd());

template <class T>
T blend(T tl, T tr, T bl, T br, float bratio, float rratio){
	//Top-Left, Top-Right, Bottom-Left, Bottom-Right, Top-Ratio, Left-Ratio
	//Imagine a square with 4 corner points, Top-Left, Top-Right, ...
	//bratio is the ratio of the bottom part, 0 means we take 0% bottom and 100% top, .3f means 30% bottom and 70% top
	//same with rratio, rratio of .6f means 60% right and 40% left
	//ratios have to be in the interval [0, 1]
	assert_fast(bratio >= 0);
	assert_fast(bratio <= 1);
	assert_fast(rratio >= 0);
	assert_fast(rratio <= 1);
	return tl * (1 - bratio) * (1 - rratio) + tr * (1 - bratio) * rratio + bl * bratio * (1 - rratio) + br * bratio * rratio;
}

template <class T, std::size_t width, std::size_t height>
void add_octave(int wave_length, std::array<std::array<T, height>, width> &array, std::uniform_real_distribution<T> &dis){
	assert_fast(wave_length >= 2);
	std::array<T, width / 2> line; //we actually want width / wave_length
	std::generate_n(begin(line), width / wave_length, [&dis]{ return dis(mt); });
	auto bl = dis(mt);
	auto br = dis(mt);
	for (unsigned y = 0; y < height; y++){
		auto bottom_ratio = (y % wave_length) / (float)wave_length;
		for (unsigned x = 0; x < width; x++){
			auto tl = line[x % wave_length];
			auto tr = line[x % wave_length + 1];
			auto right_ratio = (x % wave_length) / (float)wave_length;
			array[x][y] += blend(tl, tr, bl, br, bottom_ratio, right_ratio);
		}
		if ((y + 1) % wave_length == 0){
			line[y % wave_length] = bl;
			bl = br;
			br = dis(mt);
		}
	}
}

template <class T, int width, int height, int octaves>
std::enable_if_t<std::is_floating_point<T>::value, std::array<std::array<T, height>, width>>
get_perlin_noise(T from, T to){
	static_assert(octaves >= 0);
	static_assert(width > 0);
	static_assert(height > 0);
	assert_fast(from < to);

	std::array<std::array<T, height>, width> array;
	std::uniform_real_distribution<T> dis(from, from + (to - from) / (octaves + 1));
	std::generate(&array[0][0], &array[0][0] + width * height, [&dis]{ return dis(mt); });
	for (int i = 0; i < octaves; i++){
		add_octave(i + 2, array, dis);
	}
	return array;
}

#endif // PERLINNOISE_H
