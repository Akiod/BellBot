#include "headers/client.h"

/**
	TODO:
	Need to fix description/realname sending at intialization. The packet format must be wrong.. check RFC
**/

Client::Client(ConfigReader * c_reader)
{
	this->shut_down = false;
	this->config_reader = c_reader;
	this->action_processor = new ActionProcessor();

	this->buffer_remainder = "";
	memset(recv_buffer, '\0', BUFFER_SIZE);
	memset(&server_addr, 0, sizeof(server_addr));
}
Client::~Client()
{
	delete this->action_processor;
}
bool Client::intialize_client()
{
	client_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(client_fd < 0)
	{
		printf("Error creating socket. \n");
		return false;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(6667);

	int result = inet_pton(AF_INET, "104.200.152.162", &server_addr.sin_addr);

	if(result <= 0)
	{
		printf("pton error. \n");
		return false;
	}

	result = connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	if(result < 0)
	{
		printf("Could not connect\n");
		return false;
	}

	return true;
}
/*
	Main send function. It takes a
	string parameter and appends /r/n then calls
	the raw_send function.
*/
long int Client::real_send(std::string message)
{
	message += "\r\n";
	long int message_size = message.size();
	long int return_size = raw_send( (const char *) message.c_str(), &message_size);

	return return_size;
}
/*
	Sends [size] number of bytes from the [buffer].
	Returns -1 if it fails or the number of bytes if
	succesful.
*/
long int Client::raw_send(const char * buffer, long int * size)
{
	long int total = 0;        // how many bytes we've sent
	long int bytesleft = *size; // how many we have left to send
	long int n;

	while(total < *size) 
	{
		n = send(client_fd, buffer + total, bytesleft, 0);
		if (n == -1) 
		{ 
			break; 
		}
		total += n;
		bytesleft -= n;
	}

	*size = total; // return number actually sent here		

	return (n == -1) ? -1 : total; // return -1 on failure, 0 on success
}
long int Client::raw_recv()
{
	int result = 0;
	result = recv(client_fd, recv_buffer, BUFFER_SIZE, MSG_DONTWAIT);
	return result;
}
void Client::strip_return_characters(std::string & data)
{
	int sz = data.size();

	if(sz >= 2)
	{
		data[sz - 1] = '\0';
		data[sz - 2] = '\0';
	}
}
/**
	Parses the recv_buffer. Since we can't predict how much of a message
	we will recieve. This function checks the buffer for the packet terminating
	characters, removes them from the buffer ( and shifts the data ). Then pushes
	it to a vector we can parse.

	Packet Format:  [message]\r\n 
**/
std::vector<std::string> Client::parse_buffer()
{
	std::vector<std::string> parsed_messages;
	
	std::string str_wrapper(recv_buffer);
	str_wrapper = buffer_remainder + str_wrapper;
	buffer_remainder.clear(); buffer_remainder.resize(0);
	std::string tmp_message = "";
	memset(recv_buffer, '\0', BUFFER_SIZE);
	
	int counter = 0;
	size_t wrapper_size = str_wrapper.size();

	while(counter < wrapper_size)
	{
		if(str_wrapper[counter] == '\n' && str_wrapper[counter - 1] == '\r')
		{
			tmp_message = str_wrapper.substr(0, counter + 1);
			str_wrapper.erase(0, counter + 1);

			wrapper_size = str_wrapper.size();
			counter = 0;
			parsed_messages.push_back(tmp_message);
		}
		else
		{
			counter++;
		}
	}

	buffer_remainder = str_wrapper.substr(0, wrapper_size - 6);
	
	return parsed_messages;
}
/*
	Sends intial nick name request using
	fields specified in config.ini. 
*/
bool Client::send_user_login()
{
	std::string & nickname = config_reader->data["nickname"];
	std::string & realname = config_reader->data["realname"];
	std::string & description = config_reader->data["description"];
	std::string & channels = config_reader->data["channel"];
	
	std::string user_request = "USER " + realname + " * * :" + description;
	std::string nick_request = "NICK " + nickname;

	int rs = this->real_send(user_request);
	if(rs == -1)
		return false;
	
	rs = this->real_send(nick_request);
	if(rs == -1)
		return false;


	std::vector<std::string> channelList = split(",", channels);

	for(int i = 0; i < channelList.size(); i++)
		this->real_send("JOIN " + channelList[i]);

	return true;
}
/**
	Main receive loop. Receives bytes from the server and
	stores it in a buffer, then parses the buffer into
	human readable packets. Before looping it sends the 
	the login credentials to the server.

	The nested white loop for the 'action_packet_queue' is
	used as a bridge between the network output and the
	action processor. It allows 
**/
void Client::run_client()
{
	if(this->send_user_login())
	{
		std::vector<std::string> parsed_messages;

		while(1)
		{
			int result = this->raw_recv();


			if(result != -1)
			{
				parsed_messages = this->parse_buffer();

				for(int i = 0; i < parsed_messages.size(); i++)
				{
					this->strip_return_characters(parsed_messages[i]);
					//BELLOUTN("[RECV] %s", parsed_messages[i].c_str());

					if(!this->action_processor->do_action(parsed_messages[i], this->action_packet_queue))
					{
						this->shut_down = true;
					}

					while(!this->action_packet_queue.empty())
					{
						std::string & tmp_packet = this->action_packet_queue.front();
						this->real_send(tmp_packet);
						//BELLOUTN("[SENT] %s", tmp_packet.c_str());
						this->action_packet_queue.pop();
					}
				}

				parsed_messages.clear();
			}

			
			if(shut_down) break;

			usleep(1000); // so it doesn't eat up a whole core
		}
	}
}
