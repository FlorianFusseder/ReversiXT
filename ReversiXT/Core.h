#ifndef CORE_H
#define CORE_H



#include <iostream>
#include <string>
#include <sstream>
#include <random>


#include "Spiel.h"
#include "Turns.h"
#include "ExecTurn.h"
#include "ScoreCollector.h"
#include "AlgorithmBase.h"
#include "HelperFunctions.h"

#include "Networking.h"

#include "clock.h"



class Core
{
private:
	
	bool debug;
	bool noSort;

	std::string ip;
	std::string port;


	std::string mapname = "maps/2013_comp_5_2p.map";

	//Gamedata
	ff::Spiel GameData;


	//algo..
	ms::AlgorithmBase turnAlgo;


	int myplayernumber;

	void networkmatch();

	uint32_t convertSpecial(ff::turnPos turn); //TODO: work with not only takin defaults...
	void execNetTurn(ff::turnPos turn, int specialfield, int playernumber);



	void basicBotmatch();
	void randomBotmatch();
	void testTurn();

	void testBotmatch(int depth, ff::Spiel gData);

public:
	Core(bool Debug, bool NoSort, std::string Mapname, std::string IP, std::string Port);
	~Core();

	void Run();

};


#endif //CORE_H