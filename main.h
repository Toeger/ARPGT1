#ifndef MAIN_H
#define MAIN_H

#include <chrono>

#ifndef ARTDIR //set -DARTDIR="some/folder" to the folder that contains "art" and "data"
#define ARTDIR "."
#endif //ARTDIR

namespace Config {
	const auto lfps = 30; //logical frames per second, independent of graphical fps
	const auto logical_frame_duration = std::chrono::milliseconds(1000) / lfps;
} // namespace Config

#endif // MAIN_H
