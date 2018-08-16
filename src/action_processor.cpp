#include "headers/action_processor.h"
#include "headers/common.h"

/**
	*Make a proper system for packet parsing..
		*Possibly by tokens. get until 0x20 -> erase ->.. etc
**/

ActionProcessor::ActionProcessor()
{
	this->text_processor = new TextProcessor();
	this->entrance_controller = new EntranceController();
}
ActionProcessor::~ActionProcessor()
{
	delete this->text_processor;
	delete this->entrance_controller;
}
bool ActionProcessor::do_action(std::string data, std::queue<std::string> & packet_queue)
{
	if(data.empty()) //ignore empty packet
		return true;

	std::string prefix, command;

	if(data[0] == ':')
	{
		prefix = text_processor->extract_token(data); // Usually the client's username

		if(prefix.empty())
			return true;
	}

	command = text_processor->extract_token(data); // Usually the command JOIN/QUIT/PRIVMSG etc

	if(command.empty())
		return true;

	if(command == "PING") // Sends PONG to prevent connection timeout 
	{
		packet_queue.push("PONG " + data);
	}
	else if(command == "PRIVMSG")
	{
		const std::string sender = text_processor->extract_token(data);
		std::string reply = "";

		switch(sender[0]) // Checks if sender is from a channel or pm.
		{
			case '#': // Message receieved in a channel
				
				break;
			default: //Message receieved in a private message
				
				break;
		}
	}
	else if(command == "QUIT")
	{
		entrance_controller->incEntrance(prefix, 1);
	}
	else if(command == "JOIN")
	{
		entrance_controller->incEntrance(prefix, 0);
	}
	else if(command == "PART")
	{
		entrance_controller->incEntrance(prefix, 1);
	}
	
	return true;
}
