#include "network.h"
#include "ecs/utility.h"
#include "utility/asserts.h"
#include "utility/casts.h"

#include <arpa/inet.h>
#include <array>
#include <atomic>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

namespace Config {
	const auto host_name = "togersoft.com";
	const auto host_port = 12345;
	const std::size_t MAX_UDP_PAYLOAD = 512;
} // namespace Config

static std::thread network_thread;
static std::atomic<bool> network_should_quit{false};

static void handle(char *buffer, std::size_t size) {
	std::cout << std::string(buffer, buffer + size) << '\n' << std::flush;
}

static void run_network() {
	ON_SCOPE_EXIT(/*TODO: push "network down" event */);
	//connect
	hostent *host;
	host = gethostbyname(Config::host_name);
	if (host == nullptr) {
		std::cerr << "failed resolving " << Config::host_name << '\n';
		return;
	}
	sockaddr_in server = {};
	server.sin_family = AF_INET;
	server.sin_port = Config::host_port;
	memcpy(&server.sin_addr, host->h_addr_list, sizeof server.sin_addr);
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		perror("Failed creating socket");
		return;
	}
	ON_SCOPE_EXIT(close(fd););
	//set timeout
	{
		timeval timeout = {};
		timeout.tv_usec = 100000; //should be 100ms
		if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout) < 0) {
			perror("Error setting timeout");
			return;
		}
	}
	//login
	{
		auto logintext = "LOGIN";
		if (sendto(fd, logintext, sizeof logintext, 0, any_cast<sockaddr>(&server), sizeof server) != sizeof logintext) {
			perror("Failed sending data");
			return;
		}
	}
	//wait for messages by the server, parse them and put them in the event queue
	std::array<char, Config::MAX_UDP_PAYLOAD> buffer;

	socklen_t socket_length = sizeof server;
	while (!network_should_quit) {
		auto size = recvfrom(fd, buffer.data(), buffer.size(), 0, any_cast<sockaddr>(&server), &socket_length);
		if (size == -1) {
			//perror("Failed receiving data");
			continue;
		}
		handle(buffer.data(), size);
	}
	//todo: logout
}

void Network::run() {
	assert_fast(!network_thread.joinable());
	network_thread = std::thread(run_network);
}

void Network::update() {
	//TODO: send actions of this client to server
}

void Network::stop() {
	//stop the network thread (logout)
	assert_fast(network_thread.joinable());
	network_should_quit = true;
	network_thread.join();
	network_should_quit = false;
}

Network::Network_thread::Network_thread() {
	Network::run();
}

Network::Network_thread::~Network_thread() {
	Network::stop();
}
