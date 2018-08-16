#ifndef _ENTRANCE_H_
#define _ENTRANCE_H_

#include "common.h"

class EntranceUser
{
	public:
	std::string name;
	unsigned int joins;
	unsigned int quits;

	EntranceUser(std::string n, unsigned int j, unsigned int q);
};
class EntranceController
{
	std::vector<EntranceUser *> entranceList;

	public:
	EntranceController();
	~EntranceController();
	void loadEntranceFile();
	void saveEntranceFile();
	bool entranceUserExists(std::string user);
	void incEntrance(std::string user, char type);
	EntranceUser * addEntrance(std::string user);
	EntranceUser * findEntrance(std::string user);
	
	std::string stripName(std::string user);
};

#endif
