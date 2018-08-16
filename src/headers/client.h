#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <algorithm>
#include <queue>

#include "common.h"
#include "creader.h"
#include "action_processor.h"

#define BUFFER_SIZE 512

class Client
{
	ActionProcessor * action_processor;
	std::queue<std::string> action_packet_queue;
	ConfigReader * config_reader;

	char recv_buffer[BUFFER_SIZE];
	std::string buffer_remainder;

	bool shut_down;
	struct sockaddr_in server_addr;
	int client_fd;

	public:
		Client(ConfigReader * c_reader);
		~Client();

		bool intialize_client();
		bool send_user_login();

		long int real_send(std::string message);
		long int raw_send(const char * buffer, long int * size);
		long int raw_recv();

		std::vector<std::string> parse_buffer();

		void strip_return_characters(std::string & str);
		void run_client();
};


#endif
