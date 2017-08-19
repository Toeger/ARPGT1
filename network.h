#ifndef NETWORK_H
#define NETWORK_H

#include <vector>

namespace Network {
	//represents something that happened, such as a character moving or using a skill
	struct Event {
		//TODO: think about how to represent various events
		//use some kind of tagged union? or separate containers for different types of events?
	};

	//RAII object that starts and stops the network thread, must not have more than 1 of these
	struct Network_thread {
		Network_thread();
		~Network_thread();
	};

	//start the network thread (login, get the world state, get regular updates)
	void run();
	//stop the network thread (logout)
	void stop();
	//get latest network events, clearing the queue
	std::vector<Event> get_events();
	//send actions of this client to server
	void update();
} // namespace Network

#endif // NETWORK_H
