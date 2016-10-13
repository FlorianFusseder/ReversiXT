#include <iostream>
#include "Core.h"

int main(int argc, char* argv[]){

	std::string helptext =
		"Usage: ReversiXT [Options]\n"
		"Options:\n"
		"-d                 Debug flag\n"
		"-h                 Display this helptext\n"
		"-i <IP>           Specify IP to connect to. 127.0.0.1 if not specified\n"
		"-p <port>          Specify Server-Port. 7777 if not specified\n"
		"-nosort            Don't sort";

	std::cout <<
		"ReversiXT client / bot for YIMB" << std::endl <<
		"Group 10 - Developed by: Florian Fusseder, Peter Repukat, Sebastian Sidortschuck" << std::endl << std::endl;



	bool Debug = false;
	bool noSort = false;
	std::string mapname;
	std::string ip = "127.0.0.1";
	std::string port = "7777";

	for (int i = 0; i < argc; i++)
	{
		if (!strcmp(argv[i], "-d"))
		{
			Debug = true;
			std::cout << "Debug flag was set\n";
		}
		if (!strcmp(argv[i], "-nosort"))
		{
			noSort = true;
			std::cout << "NoSort flag was set\n";
		}
		if (!strcmp(argv[i], "-h"))
		{
			std::cout << helptext;
			return 0;
		}
		if (!strcmp(argv[i], "-i"))
		{
			if (argc <= i + 1)
			{
				std::cout << "-i needs parameter <ip>" << std::endl;
				return EXIT_SUCCESS;
			}
			else if (strstr(argv[i + 1], "-") != NULL)
			{
				std::cout << "-i needs parameter <ip>" << std::endl;
				return EXIT_SUCCESS;
			}
			else {
				ip = argv[i + 1];
			}
		}
		if (!strcmp(argv[i], "-p"))
		{
			if (argc <= i + 1)
			{
				std::cout << "-p needs parameter <port>" << std::endl;
				return EXIT_SUCCESS;
			}
			else if (strstr(argv[i + 1], "-") != NULL)
			{
				std::cout << "-p needs parameter <mapname>" << std::endl;
				return EXIT_SUCCESS;
			}
			else {
				port = argv[i + 1];
			}
		}
	}

	Core* core = new Core(Debug, noSort, mapname, ip, port);

	core->Run();

	delete core;
	return EXIT_SUCCESS;
}
