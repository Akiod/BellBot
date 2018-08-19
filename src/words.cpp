#include "headers/words.h"

#include <fstream>
#include <string>

WordController::WordController()
{
	
}
WordController::~WordController()
{
	for(int i = 0; i < wordLogs.size(); i++)
	{
		delete wordLogs[i];
	}
	wordLogs.clear();
}
void WordController::log(WordUser * wordUser)
{
	std::ofstream file;

	file.open("word_log", std::ios_base::app);
	file << wordUser->username << "," << wordUser->channel << "," << wordUser->word << "," << wordUser->timestamp << std::endl;
	file.close();
}
void WordController::detect(std::string msg, std::string user, std::string channel)
{
	std::vector<std::string> containedWords = this->getDetectedWords(msg);
	for(int w = 0; w < containedWords.size(); w++)
	{
		wordLogs.push_back(new WordUser(user, channel, containedWords[w], (unsigned int) time(NULL)));
		this->log(wordLogs[wordLogs.size() - 1]);
	}
	containedWords.clear();
}
void WordController::loadConfigTargets(std::string config_data)
{
	std::vector<std::string> tWords = split(",", config_data);

	for(int i = 0; i < tWords.size(); i++)
		targetWords.push_back(tWords[i]);
	
	BELLOUTN(std::to_string(targetWords.size()) + " target words have been loaded.");
}
std::vector<std::string> WordController::getDetectedWords(std::string msg)
{
	std::vector<std::string> words;
	int charCounter = 0;

	for(int w = 0; w < targetWords.size(); w++)
	{
		for(int c = 0; c < msg.size(); c++)
		{
			if(msg[c] == targetWords[w][charCounter])
				charCounter++;
			else
				charCounter = 0;

			if(charCounter == (targetWords[w].size() - 1))
			{
				words.push_back(targetWords[w]);
			}
		}
	}
	return words;
}
