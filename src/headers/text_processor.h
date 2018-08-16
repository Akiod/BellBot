#ifndef _TEXT_PROCESSOR_H
#define _TEXT_PROCESSOR_H

#include <iostream>
#include <string>
#include <cctype>


class TextProcessor
{
	public:
		std::string parse_real_username(std::string & username);
		std::string extract_token(std::string & packet);
		
};

#endif
