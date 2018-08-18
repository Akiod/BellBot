#ifndef _ACTION_PROCESSOR_H
#define _ACTION_PROCESSOR_H

#include "text_processor.h"
#include "entrance.h"
#include "words.h"

#include <vector>
#include <queue>

struct ActionPart
{
	std::string name;
	int break_loc;
};
class ActionProcessor
{
	public:
	TextProcessor * text_processor;
	EntranceController * entrance_controller;
	WordController * word_controller;
	
	ActionProcessor();
	~ActionProcessor();
	bool do_action(std::string data, std::queue<std::string> & packet_queue);
};

#endif
