#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include "utility/asserts.h"

#include <algorithm>
#include <array>
#include <numeric>
#include <random>
#include <type_traits>

static std::random_device rd;
static std::mt19937_64 mt(rd());

template <class T>
T blend(T tl, T tr, T bl, T br, float bratio, float rratio) {
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare" //for some reason the compiler flag -Wno-sign-compare has no effect on this function
template <class T, std::size_t width, std::size_t height>
void add_octave(int wave_length, std::array<std::array<T, height>, width> &array, std::uniform_real_distribution<T> &dis) {
	assert_fast(wave_length >= 2);
	std::array<T, width / 2 + 2> top_line;    //we actually want width / wave_length
	std::array<T, width / 2 + 2> bottom_line; //we actually want width / wave_length
	std::generate_n(begin(top_line), width / wave_length + 1, [&dis] { return dis(mt); });
	std::generate_n(begin(bottom_line), width / wave_length + 1, [&dis] { return dis(mt); });
	for (int y = 0; y < height; y++) {
		const auto bottom_ratio = (y % wave_length) * 1.f / wave_length;
		for (int x = 0; x < width; x++) {
			const auto tl = top_line[x / wave_length];
			const auto tr = top_line[x / wave_length + 1];
			const auto bl = bottom_line[x / wave_length];
			const auto br = bottom_line[x / wave_length + 1];
			const auto right_ratio = (x % wave_length) * 1.f / wave_length;
			const auto value = blend(tl, tr, bl, br, bottom_ratio, right_ratio);
			array[x][y] += value;
		}
		if ((y + 1) % wave_length == 0) {
			std::copy(begin(bottom_line), begin(bottom_line) + width / wave_length + 2, begin(top_line));
			std::generate_n(begin(bottom_line), width / wave_length + 2, [&dis] { return dis(mt); });
		}
	}
}
#pragma clang diagnostic pop

template <class T>
void add_octave(int width, int height, int wave_length, std::vector<T> &array, std::uniform_real_distribution<T> &dis) {
	assert_fast(wave_length >= 2);
	std::vector<T> top_line(width / wave_length + 2);
	std::vector<T> bottom_line(width / wave_length + 2);
	std::generate(begin(top_line), end(top_line), [&dis] { return dis(mt); });
	std::generate(begin(bottom_line), end(bottom_line), [&dis] { return dis(mt); });
	for (int y = 0; y < height; y++) {
		const auto bottom_ratio = (y % wave_length) * 1.f / wave_length;
		for (int x = 0; x < width; x++) {
			const auto tl = top_line[x / wave_length];
			const auto tr = top_line[x / wave_length + 1];
			const auto bl = bottom_line[x / wave_length];
			const auto br = bottom_line[x / wave_length + 1];
			const auto right_ratio = (x % wave_length) * 1.f / wave_length;
			const auto value = blend(tl, tr, bl, br, bottom_ratio, right_ratio);
			array[x + width * y] += value;
		}
		if ((y + 1) % wave_length == 0) {
			std::swap(top_line, bottom_line);
			std::generate(begin(bottom_line), end(bottom_line), [&dis] { return dis(mt); });
		}
	}
}

template <class T, int width, int height, int octaves>
std::enable_if_t<std::is_floating_point<T>::value, std::array<std::array<T, height>, width>> get_perlin_noise(T from, T to, int start = 0) {
	static_assert(octaves >= 0, "");
	static_assert(width > 0, "");
	static_assert(height > 0, "");
	assert_fast(from < to);
	assert_fast(start <= octaves);

	std::array<std::array<T, height>, width> array;
	std::uniform_real_distribution<T> dis(from, from + (to - from) / (octaves - start + 1));
	//std::generate(&array[0][0], &array[0][0] + width * height, [&dis]{ return dis(mt); });
	std::fill(&array[0][0], &array[0][0] + width * height, 0);
	for (int i = start; i < octaves; i++) {
		add_octave(i + 2, array, dis);
	}
	return array;
}

template <class T>
std::enable_if_t<std::is_floating_point<T>::value, std::vector<T>> get_perlin_noise(int width, int height, int octaves, T from, T to, int start = 0) {
	assert_fast(octaves >= 0);
	assert_fast(width > 0);
	assert_fast(height > 0);
	assert_fast(from < to);
	assert_fast(start <= octaves);

	std::vector<T> array(width * height, 0);
	std::uniform_real_distribution<T> dis(from, from + (to - from) / (octaves - start + 1));
	for (int i = start; i < octaves; i++) {
		add_octave(width, height, i + 2, array, dis);
	}
	return array;
}

#endif // PERLINNOISE_H
