#ifndef MAIN_H
#define MAIN_H

#include <chrono>

struct Window;

namespace Config {
	const auto lfps = 30; //logical frames per second, independent of graphical fps
	const auto logical_frame_duration = std::chrono::milliseconds(1000) / lfps;
}

namespace Global{
	extern Window *window;
}

#endif // MAIN_H
