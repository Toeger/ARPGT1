#ifndef MAIN_H
#define MAIN_H

#include <chrono>

namespace Config{
	const auto lfps = 30; //logical frames per second, independent of graphical fps
	const auto logical_frame_duration = std::chrono::milliseconds(1000) / lfps;
}

#endif // MAIN_H
