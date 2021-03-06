#include "perlin_test.h"
#include "ecs/log.h"
#include "ecs/utility.h"
#include "graphics/perlinnoise.h"
#include "utility/asserts.h"

#include <iostream>
#include <irrlicht/irrlicht.h>

constexpr int max_i = 64;

template <class T>
void drop(T *t) {
	t->drop();
}

namespace std {
	template <class T>
	unique_ptr(T *, void (*)(T *))->unique_ptr<T, void (*)(T *)>;
}

template <int i = 0>
void run_perlin() {
	//not sure how to automatically test perlin noise, so just dump an image for now and manually check it
	const int width = 1024;
	const int height = 1024;
	const int min = 0;
	const int max = 255;

	auto noise = get_perlin_noise<float, width, height, i>(0, 255);
	auto render_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(800, 600), 32, false, false, true, nullptr);
	std::unique_ptr rd_dropper{render_device, drop};
	auto video_driver = render_device->getVideoDriver();
	auto image = video_driver->createImage(irr::video::ECOLOR_FORMAT::ECF_R8G8B8, {width, height});
	std::unique_ptr id_dropper{image, drop};
	std::unique_ptr idd{image, drop};
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			auto &value = noise[x][y];
			assert_fast(value >= min);
			assert_fast(value <= max);
			image->setPixel(x, y, {value, value, value});
		}
	}
	bool success = video_driver->writeImageToFile(image, "media/perlin.png");
	assert_fast(success);
}

template <int i = 0>
void multi_test_perlin() {
	run_perlin<i>();
	multi_test_perlin<i + 1>();
}

template <>
void multi_test_perlin<max_i>() {}

void test_perlin() {
	//multi_test_perlin();
	const int width = 1024;
	const int height = 1024;
	const float min = 0;
	const float max = 1;
	const float separator = 0.47f;

	auto noise = get_perlin_noise<float, width, height, 40>(min, max, 20);
	float minimum = 255, maximum = 0;
	for (const auto &array : noise) {
		auto minmax = std::minmax_element(begin(array), end(array));
		minimum = std::min(minimum, *minmax.first);
		maximum = std::max(maximum, *minmax.second);
	}
	auto sigmoid = [ minimum, maximum, cutoff = 1 - separator, steepness = 64 ](float f) {
		auto normal_f = (f - minimum) / (maximum - minimum);
		return 1 / (1 + std::exp(-steepness * (normal_f - cutoff)));
	};

	std::transform(&noise[0][0], &noise[0][0] + width * height, &noise[0][0], sigmoid);
	auto render_device = irr::createDevice(irr::video::EDT_NULL);
	std::unique_ptr rd_dropper{render_device, drop};
	auto video_driver = render_device->getVideoDriver();
	auto image = video_driver->createImage(irr::video::ECOLOR_FORMAT::ECF_R8G8B8, {width, height});
	std::unique_ptr image_dropper{image, drop};
	auto inverted_image = video_driver->createImage(irr::video::ECOLOR_FORMAT::ECF_R8G8B8, {width, height});
	std::unique_ptr inverted_image_dropper{inverted_image, drop};
	auto square = [](auto x) { return x * x; };
	for (int y = 0; y < height; y++) {
		for (int x_ = 0; x_ < width; x_++) {
			auto &value = noise[x_][y];
			const auto x = width - 1 - x_;
			assert_fast(value >= min);
			assert_fast(value <= max);
			//unsigned char c = value < separator ? 0 : 255;
			unsigned char c = value * 255;
			image->setPixel(x, y, {0, c, c, c});
			unsigned char inv_c = 255 - c;
			inverted_image->setPixel(x, y, {0, inv_c, inv_c, inv_c});
			if (square(x) + square(y) < 10 * 10 * 10) {
				inverted_image->setPixel(x, y, {0, 255, 0, 0});
			}
			if (square(width - x) + square(y) < 10 * 10 * 10) {
				inverted_image->setPixel(x, y, {0, 0, 255, 0});
			}
		}
	}
	bool is_bmp_writable = false;
	Log::log_debug() << video_driver->getImageWriterCount();
	for (auto image_writer_index = 0u; image_writer_index < video_driver->getImageWriterCount(); image_writer_index++) {
		auto image_write = video_driver->getImageWriter(image_writer_index);
		Log::log_debug() << image_write->getDebugName();
		if (image_write->isAWriteableFileExtension(".bmp")) {
			is_bmp_writable = true;
			break;
		}
	}
	assert_fast(is_bmp_writable);
	bool success = video_driver->writeImageToFile(image, "/tmp/perlin_map.bmp");
	assert_fast(success);
	success = video_driver->writeImageToFile(inverted_image, "/tmp/inverted_perlin_map.png");
	assert_fast(success);
}
