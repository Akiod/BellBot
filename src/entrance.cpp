#include "headers/entrance.h"

#include <fstream>

EntranceController::EntranceController()
{
	this->loadEntranceFile();
}
EntranceController::~EntranceController()
{
	for(int i = 0; i < entranceList.size(); i++)
	{
		delete entranceList[i];
	}

	entranceList.clear();
}
/*
	Loads the entrance file contents into
	the entranceList.
	
	@Example:
		Abi 112 30
		[name] [joins] [quits]
*/
void EntranceController::loadEntranceFile()
{
	EntranceUser * ptr = NULL;
	std::fstream file;
	std::string line;
	std::vector<std::string> tokens;

	file.open("entrance_log", std::fstream::in);
	
	while(std::getline(file, line))
	{
		if(line.empty())
			continue;
		if(line[0] == '#')
			continue;
	
		tokens = split(" ", line);
		if(tokens.size() != 4 || !is_number(tokens[1]) || !is_number(tokens[2]))
			continue;

		ptr = findEntrance(tokens[0]);
		if(ptr == NULL)
		{
			ptr = addEntrance(tokens[0]);
		}
		
		int type = stoi(tokens[2]);
		if(type == 0)
			ptr->joins++;
		else if(type == 1)
			ptr->quits++;

		tokens.clear();	
	}

	BELLOUTN(std::to_string(entranceList.size()) + " entrance entries loaded.");

	file.close();
}
void EntranceController::appendEntranceLog(std::string user, std::string channel, char type)
{
	unsigned int epoch_time = (unsigned) time(NULL);
	std::ofstream file;

	file.open("entrance_log", std::ios_base::app);
	file << user << " " << epoch_time << " " << (int)type << " " << channel << std::endl;
	file.close();
}
std::string EntranceController::stripName(std::string user)
{
	std::string ret = "";
	for(int i = 1; i < user.size(); i++)
	{
		if(user[i] == '!')
			break;
		ret += user[i];
	}
	return ret;
}
void EntranceController::incEntrance(std::string u, std::string channel, char type)
{
	std::string user = stripName(u);
	channel = channel.substr(0, channel.size() - 2);

	EntranceUser * ptr = NULL;

	if(!entranceUserExists(user))
		ptr = addEntrance(user);
	else
		ptr = findEntrance(user);

	
	if(type == 0)
		ptr->joins++;
	else if(type == 1)
		ptr->quits++;

	std::cout << ptr->name << " " << ptr->joins << " joins " << ptr->quits << " quits. [ List size: " << entranceList.size() << " ]."  << std::endl;

	this->appendEntranceLog(ptr->name, channel, type);
}
bool EntranceController::entranceUserExists(std::string user)
{
	for(int i = 0; i < this->entranceList.size(); i++)
	{
		if(this->entranceList[i]->name == user)
			return true;
	}

	return false;
}
EntranceUser * EntranceController::addEntrance(std::string user)
{
	this->entranceList.push_back(new EntranceUser(user, 0, 0));
	return this->entranceList[entranceList.size() - 1];
}
EntranceUser * EntranceController::findEntrance(std::string user)
{
	for(int i = 0; i < this->entranceList.size(); i++)
	{
		if(this->entranceList[i]->name == user)
			return entranceList[i];
	}

	return NULL;
}
EntranceUser::EntranceUser(std::string n, unsigned int j, unsigned int q)
{
	this->name = n;
	this->joins = j;
	this->quits = q;
}
