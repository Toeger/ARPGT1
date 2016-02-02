#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <cstring>

#include "network.h"
#include "Utility/asserts.h"

namespace Config{
	const auto host_name = "togersoft.com";
	const auto host_port = 12345;
}

static std::thread network_thread;

static void run_network(){
	//connect
	hostent *host;
	host = gethostbyname(Config::host_name);
	if (!host){
		std::cerr << "failed resolving " << Config::host_name << '\n';
	}
	sockaddr_in server = {};
	server.sin_family = AF_INET;
	server.sin_port = Config::host_port;
	memcpy(&server.sin_addr, host->h_addr_list, sizeof server.sin_addr);
	//login

	//wait for messages by the server, parse them and put them in the event queue
	for (;;){

	}
}

void Network::run()
{
	assert_fast(!network_thread.joinable());
	network_thread = std::thread(run_network);
}
