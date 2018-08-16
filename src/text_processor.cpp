#include "headers/text_processor.h"

std::string TextProcessor::parse_real_username(std::string & username)
{
	std::string real_username = "";
	
	for(int i = 1; i < username.size(); i++)
	{
		if(username[i] == ' ')
			break;
		else
			real_username += username[i];
	}

	return real_username;
}
/**
	Extracts token from a packet. The terminating character is
	a 0x20 ( space ). Erases the token from the original packet
	and returns the token.
**/
std::string TextProcessor::extract_token(std::string & packet)
{
	std::string extracted = "";
	int break_counter = 0;

	for(char c : packet)
	{
		if(isblank(c))
		{
			break_counter++;
			break;
		}

		extracted += c;
		break_counter++;
	}
		
	
	packet.erase( packet.begin(), packet.begin() + break_counter);

	return extracted;
}
