#include "Core.h"


Core::Core(bool Debug, bool NoSort, std::string Mapname, std::string IP, std::string Port)
{
	if (Mapname == "debug")
	{
		debug = true;

	} else {
		debug = false;
		mapname = Mapname;
	}
	if (Debug)
		debug = true;
	noSort = NoSort;
	ip = IP;
	port = Port;
}


Core::~Core()
{
}



void Core::Run()
{
	
		networkmatch();
#ifdef _WIN32
		if (IsDebuggerPresent())
		{
			std::cout << "Debugger found! Press return to exit!";
			std::cin.ignore();
		}
#endif
		return;
}

uint32_t Core::convertSpecial(ff::turnPos turn)
{
		return turn.special;
}


void Core::execNetTurn(ff::turnPos turn, int specialfield, int playernumber)
{
	if (specialfield == 20 || specialfield == 21)
	{
		specialfield -= 19;
		repu::execTurn(GameData, GameData.getPlayer(playernumber), turn, short(specialfield));
	}
	else if (specialfield > 0 && specialfield < 9)
	{
		repu::execTurn(GameData, GameData.getPlayer(playernumber), turn, char(specialfield + 48));
	}
	else {
		repu::execTurn(GameData, GameData.getPlayer(playernumber), turn);
	}
}

void Core::networkmatch()
{

	repu::Networking net;
	repu::netwBuf buff;



	//connecting phase..

	//create socket, connect, and transmit groupno
	if (debug)
		std::cout << "connecting and tramsitting groupno\n";
	if (net.mConnect(ip, port) < 1)
		return;

	//recieving map...
	if (debug)
		std::cout << "recieving map\n";
	if (net.mRecieve(buff) < 1)
		return;

	//if error...
	if (buff.buf()[0] != 2)
	{
		std::cout << "Excpected type map (0x02), but type was: " << (int)buff.buf()[0] << std::endl;
	}


	//put map in a from "Spiel" readable format...
	std::stringstream mapstr;
	for (unsigned int i = 5; i < buff.size(); i++)
		mapstr << char(buff.bufV().at(i));


	GameData.einlesen(mapstr);								//convert and read map
	if (debug)
		GameData.outputBrett();									//print map out

	if (debug)
		std::cout << "recieving playerno\n";
	if (net.mRecieve(buff) < 1)								//recieve playerno
		return;

	if (buff.buf()[0] != 3)
	{
		std::cout << "Excpected type playernumber (0x03), but type was: " << (int)buff.buf()[0] << std::endl;
	}
	myplayernumber = (uint32_t)buff.buf()[5];


	//\connecting phase..

	if (debug)
		std::cout << std::endl << "Starting game as player " << myplayernumber << " on Server: " << ip << ":" << port << std::endl;

	int type = 0;
	ff::turnPos turn;
	ff::turnPos turn2 = ff::turnPos{ -1, -1 };
	uint32_t timelimit;
	int depthlimit;
	int specialfield = 0;
	int playernumber;
	int turnVector = 0;
	bool timeZero = false;

	int nTurns;

	if (debug)
		turnAlgo.setDebug(true);
	if (noSort)
		turnAlgo.setNoSort(true);

	while (type < repu::Networking::end_phase1)
	{
		//recieve data
		if (net.Recieve2(buff, type, timelimit, depthlimit, turn, specialfield, playernumber) < 1)
			break;

		repu::clock timeTurns;
		switch (type)
		{
		case repu::Networking::turn_prompt:
			if (debug)
				std::cout << "server prompting for turn... depthlimit: " << depthlimit << " timelimit: " << timelimit <<"\n\n";

			if (timelimit == uint32_t(0))
				timeZero = true;


			//select turn
			if (debug)
			{

				timeTurns.restart();
				ff::startTurns(GameData, myplayernumber);				//get possible turns // dont calculate this in to time //or subtract if we use a timelimit
				nTurns = GameData.getPlayer(myplayernumber).getAllowedTurns().size() +
					GameData.getPlayer(myplayernumber).getAllowedOverwriteTurns().size() +
					GameData.getPlayer(myplayernumber).getAllowedOverwriteTurnsX().size();
				timelimit -= (uint32_t)timeTurns.getAsMilSecs();
			}

			if (timeZero)
			{
				if (depthlimit == 0)
				{
					depthlimit = 1;
				}
				if (debug)
					std::cout << "searching for turn using regular algorithm\n";
				turn = turnAlgo.MiniMaxPos(GameData, myplayernumber, depthlimit);
			}
			else {
				if (debug)
					std::cout << "searching for turn using iterative deepening\n";
				turn = turnAlgo.MiniMaxDeep(GameData, myplayernumber, timelimit);
			}


			if (debug)
				std::cout << "\nTook me " << timeTurns.getAsSecs() << " seconds to compute and find turn out of "
				<< nTurns << "\n\n";  


			int numTurnsPlayer;
			turn2 = turn;

			//\select turn


			specialfield = convertSpecial(turn);

			if (debug)
				std::cout << "sending turn to server...  X: " << turn.pos_X << " Y: " << turn.pos_Y  << " Special: " << specialfield << std::endl;
			net.sendTurn(turn,specialfield);
			break;


		case repu::Networking::turn:
			if (debug)
				std::cout << "server sending turn: Playernumber: " << playernumber << " X=" << turn.pos_X << " Y=" << turn.pos_Y << " Special: " << specialfield << std::endl;

				if (debug)
					timeTurns.restart();

				ff::startTurns(GameData, playernumber);

				//search for the turn
				turnVector = 0;
				turn2 = ff::turnPos{ -1, -1 };

				for (unsigned int i = 0; i < GameData.getPlayer(playernumber).getAllowedTurns().size(); i++)
				{
					turn2 = GameData.getPlayer(playernumber).getAllowedTurns().at(i);
					if (turn2.pos_X == turn.pos_X && turn2.pos_Y == turn.pos_Y)
					{
						//did we find it?
						turnVector = 1;
						break;
					}
				}
				if (turnVector == 0)
				{
					for (unsigned int i = 0; i < GameData.getPlayer(playernumber).getAllowedOverwriteTurns().size(); i++)
					{
						turn2 = GameData.getPlayer(playernumber).getAllowedOverwriteTurns().at(i);
						if (turn2.pos_X == turn.pos_X && turn2.pos_Y == turn.pos_Y)
						{
							//did we find it?
							turnVector = 2;
							break;
						}
					}
				}
				if (turnVector == 0)
				{
					for (unsigned int i = 0; i < GameData.getPlayer(playernumber).getAllowedOverwriteTurnsX().size(); i++)
					{
						turn2 = GameData.getPlayer(playernumber).getAllowedOverwriteTurnsX().at(i);
						if (turn2.pos_X == turn.pos_X && turn2.pos_Y == turn.pos_Y)
						{
							//did we find it?
							turnVector = 3;
							break;
						}
					}
				}
			//}

				if (debug && turnVector > 1)
					std::cout << "turn was overwrite or overwriteX turn\n";

			if (turnVector == 0)								//did we find it?
			{
				std::cout << "ERROR! Turn that should be executed could not be found!\n";
				std::cout << "\n\n";

				GameData.outputLegitTurns();

				std::cout << std::endl;

				GameData.outputBrett();
				std::cout << "\n\n";
				return;

			} else {
				numTurnsPlayer = GameData.getPlayer(playernumber).getAllowedTurns().size() +
					GameData.getPlayer(playernumber).getAllowedOverwriteTurns().size() +
					GameData.getPlayer(playernumber).getAllowedOverwriteTurnsX().size();


				if (debug){
					std::cout << "vorher" << std::endl;
					GameData.outputBrett();
					GameData.outputLegitTurns();
				}
				

				execNetTurn(turn2, specialfield, playernumber);							//if yes, we execute!

				

				if (debug){
					std::cout << "nachher" << std::endl;
					GameData.outputBrett();
				}

			}

			break;

		case repu::Networking::disquali:
			HelperFunctions::disqualifyPlayer(playernumber, GameData);
			std::cout << "player " << playernumber << " is disqualified!" << std::endl;
			if (debug)
			{
				if (playernumber == myplayernumber)
				{
					std::cout << "whoops thats me!\nmap\n\n";
					GameData.outputBrett();
					std::cout << "\n\n";
				}
			}
			break;


		}


	}

	if (debug)
		std::cout << "type was >= repu::Networking::endphase1... moving to bomb phase\n";

		while (type < repu::Networking::end_phase2)
		{
			if (net.Recieve2(buff, type, timelimit, depthlimit, turn, specialfield, playernumber) < 1)
				break;
			if (type == repu::Networking::turn_prompt)
			{
				if (debug)
					std::cout << "server prompting for turn\n";

				turn2 = ms::AlgorithmBase::bombPos(GameData, myplayernumber);						// Beste bombeposition finden


				if (debug)
					std::cout << "sending turn to server...  X: " << turn2.pos_X << " Y: " << turn2.pos_Y << " Special: " << specialfield << std::endl;

				net.sendTurn(turn2, 0);

				if (debug)
					GameData.outputBrett();
			}
			if (type == repu::Networking::turn)
			{
				if (debug)
					std::cout << "server sending turn: Playernumber: " << playernumber << " X=" << turn.pos_X << " Y=" << turn.pos_Y << " Special: " << specialfield << std::endl;
				repu::execBomb(GameData, repu::bombStep{ GameData.getStaerke(), turn });
			}
			if (type == repu::Networking::disquali)
			{
				std::cout << "player " << playernumber << " is disqualified!" << std::endl;
			}

		}

	std::cout << "done!" << std::endl;
	std::cout << "we were player: " << myplayernumber << std::endl;
	return;
}

