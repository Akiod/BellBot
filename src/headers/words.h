#ifndef _WORDS_H_
#define _WORDS_H_

#include "common.h"

class WordUser
{
	public:
	std::string username;
	std::string channel;
	std::string word;
	unsigned int timestamp;

	WordUser(std::string u, std::string c, std::string w, unsigned int t) : username(u), channel(c), word(w), timestamp(t) { }
};

class WordController
{
	std::vector<std::string> targetWords;
	std::vector<WordUser*> wordLogs;

	public:
	WordController();
	~WordController();

	void detect(std::string msg, std::string user, std::string channel);
	std::vector<std::string> getDetectedWords(std::string msg);
};

#endif
