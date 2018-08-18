#include "headers/words.h"

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
void WordController::detect(std::string msg, std::string user, std::string channel)
{
	std::vector<std::string> containedWords = this->getDetectedWords(msg);
	for(int w = 0; w < containedWords.size(); w++)
	{
		wordLogs.push_back(new WordUser(user, channel, containedWords[w], (unsigned int) time(NULL)));
	}
	containedWords.clear();
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
