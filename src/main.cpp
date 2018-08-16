#include "headers/client.h"
#include "headers/creader.h"

int main(int args, char ** argc)
{
	ConfigReader * config_reader = new ConfigReader("config.ini");
	if(!config_reader->config_read())
	{
		delete config_reader;
		return -1;
	}

	Client * irc_client = new Client(config_reader);
	if(!irc_client->intialize_client())
	{
		delete irc_client;
		delete config_reader;

		return -1;
	}

	irc_client->run_client();

	delete irc_client;
	delete config_reader;

	return 0;
}
