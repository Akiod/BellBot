#ifndef _CONFIG_READER_
#define _CONFIG_READER_

#include <map>

class ConfigReader
{
	const char * file_name;
	int field_count;

	public:
		ConfigReader(const char * name) : file_name(name) {}
		std::map<std::string, std::string> data;

		bool config_read();
		void split(std::string & word, std::string & key, std::string & value);
		void erase_after_space(std::string & str);
		void erase_before_space(std::string & str);
};

#endif
