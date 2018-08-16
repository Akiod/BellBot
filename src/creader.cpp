#include <iostream>
#include <fstream>

#include "headers/creader.h"

/*
	Splits the config file line by the '=' character.
	Everything before the '=' is the key.
	Everything after the '=' is the value.
	Calls functions to clear spaces that follow/prefix the strings.
*/
void ConfigReader::split(std::string & word, std::string & key, std::string & value)
{
	// Split the 'word' string by the '=' character, and put the contents in the proper places.

	std::string tmp;
	int switcher = 0;
	
	for(int i = 0; i < word.size(); i++)
	{
		if(word[i] == '=')
		{
			switcher = 1;
		}
		else
		{
			switch(switcher)
			{
				case 0: key += word[i]; break;
				case 1: value += word[i]; break;
			}
		}
	}
	this->erase_after_space(key);
	this->erase_before_space(value);
	this->erase_after_space(value);
	this->erase_before_space(key);
}
/**
	Erases all spaces before the string begins.
**/
void ConfigReader::erase_before_space(std::string & str)
{
	for(int i = 0; i < str.size(); i++)
	{
		if(str[i] == ' ')
			str = str.substr(i+1, str.size());
		else
			break;
	}
}
/**
	Erases all spaces following the strings end. 
**/
void ConfigReader::erase_after_space(std::string & str)
{
	for(int i = str.size() - 1; i > 0; i--)
	{
		if(str[i] == ' ')
			str = str.substr(0, i);
		else
			break;
	}
}
bool ConfigReader::config_read()
{
	field_count = 0;

	std::ifstream config_file;
	config_file.open(file_name);

	if(!config_file.good())
		return false; //file doesn't exist

	std::string c_word, key, value;

	while(std::getline(config_file, c_word))
	{
		if(c_word[0] != '#' && c_word.size() > 0)
		{
			this->split(c_word, key, value);
			this->data[key] = value;
			key.clear(); value.clear();
		}
	}

	config_file.close();
	return true;
}

