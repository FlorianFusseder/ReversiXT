#include "AlgorithmBase.h"

namespace ms {

	AlgorithmBase::AlgorithmBase()
	{
		debug = false;
	}

	AlgorithmBase::AlgorithmBase(bool _debug, bool _noSort)
	{
		debug = _debug;
		noSort = _noSort;
	}

	AlgorithmBase::~AlgorithmBase()
	{
	}

	void AlgorithmBase::setDebug(bool _debug)
	{
		debug = _debug;
	}
	void AlgorithmBase::setNoSort(bool _noSort)
	{
		noSort = _noSort;
	}

	void AlgorithmBase::SortGame(std::vector<ff::Spiel>& games, std::vector<ff::turnPos>& turns, short player, bool isMax)
	{

		std::vector<ms::sortStruct> tempStructs;
		sortStruct tempStruct;
		for (unsigned int i = 0; i < games.size(); i++)
		{
			tempStruct.gID = i;
			tempStruct.value = ms::Evaluator::getDiff(player, games[i]);
			tempStructs.push_back(tempStruct);
		}


		if(isMax)std::sort(tempStructs.begin(), tempStructs.end(), CompareStructs);
		else std::sort(tempStructs.begin(), tempStructs.end(), CompareStructsMin);


		for (auto& temp : tempStructs){
			games.push_back(games[temp.gID]);
			turns.push_back(turns[temp.gID]);
		}
		games.erase(games.begin(), games.begin() + (games.size() / 2));
		turns.erase(turns.begin(), turns.begin() + (turns.size() / 2));

	}

	bool AlgorithmBase::CompareStructs(const sortStruct& a, const sortStruct& b)
	{
		return a.value > b.value;
	}

	bool AlgorithmBase::CompareScore(const rank& a, const rank& b){

		return a.score > b.score;

	}

	bool AlgorithmBase::CompareStructsMin(const sortStruct& a, const sortStruct& b){

		return a.value < b.value;

	}

	int AlgorithmBase::MiniMax(ff::Spiel& game, short player, int maxDepth, int depth, abp wertung) {

		if (!run)
		{
			if (debug)
				std::cout << "run flag was set to stop!\n";
			ff::startTurns(game, player);
			return ms::Evaluator::getValue(player, game);
		}

		zaehler++;
		int playerToCalculate = static_cast<int>((player - 1 + depth) % game.getPlayers().size() + 1); 
		int m_depth = depth + 1;
		if (depth % game.getPlayers().size() == 0) {
			wertung.value = INT_MIN;
		}
		else{
			wertung.value = INT_MAX;
		}
		if (!(HelperFunctions::playerIsActive(playerToCalculate, game)))
			return MiniMax(game, player, maxDepth, m_depth, wertung);

		ff::startTurns(game, playerToCalculate);

		if (depth >= maxDepth) {

			ff::startTurns(game, player);
			if (game.freeFields() - depth <= 0)
			{
				int val = ms::Evaluator::getValueSimple(player, game);
				return val;
			}

			int val = ms::Evaluator::getDiff(player, game);
			return val;

		}
		else {

			if (game.getPlayer(playerToCalculate).getAllowedTurns().size() == 0 &&
				game.getPlayer(playerToCalculate).getAllowedOverwriteTurns().size() == 0 &&
				game.getPlayer(playerToCalculate).getAllowedOverwriteTurnsX().size() == 0)
			{	
				return  MiniMax(game, player, maxDepth, m_depth, wertung);
			}

			int tempv;
			int counter = 0;
			std::vector<ff::turnPos> tempTurns;
			std::vector<ff::turnPos> fullTurns = game.getPlayer(playerToCalculate).getAllowedTurns();
			if (((fullTurns.size() < 5 && game.freeFields() < 10) || fullTurns.size() < 3) && game.getPlayer(playerToCalculate).getNumOverrideStones() > 0)
			{
				std::vector<ff::turnPos> tempTurns;
				if (game.getPlayer(playerToCalculate).getAllowedOverwriteTurns().size() > 0) { 
					tempTurns = game.getPlayer(playerToCalculate).getAllowedOverwriteTurns();
					fullTurns.insert(fullTurns.end(), tempTurns.begin(), tempTurns.end());
				}
				if (game.getPlayer(playerToCalculate).getAllowedOverwriteTurnsX().size() > 0) {
					tempTurns = game.getPlayer(playerToCalculate).getAllowedOverwriteTurnsX();
					fullTurns.insert(fullTurns.end(), tempTurns.begin(), tempTurns.end());
				}
			}

			//SORT
			std::vector<ff::Spiel> games;
			for (auto& turn : fullTurns){
				games.push_back(game);


				if (games[games.size() - 1].getMap()[turn.pos_Y][turn.pos_X].art == ff::Feld::Art::Choice)
				{

					short *stones = new short[game.getSpielerzahl()];

					for (unsigned short i = 1; i < game.getSpielerzahl(); i++)
					{
						stones[i - 1] = games[games.size() - 1].getPlayer(i).getPossibleStartPosition().size();
					}

					short biggestValPlayer = 0;

					for (unsigned short i = 1; i < game.getSpielerzahl(); i++)
					{
						if (stones[biggestValPlayer] < stones[i])
							biggestValPlayer = i;
					}

					delete[] stones;

					turn.special = biggestValPlayer + 1;

					repu::execTurn(games[games.size() - 1], games[games.size() - 1].getPlayer(playerToCalculate), turn, char(turn.special + 48));
				}
				else if (games[games.size() - 1].getMap()[turn.pos_Y][turn.pos_X].art == ff::Feld::Art::Bonus)
				{
					
					if (game.getStaerke() > 1)
						turn.special = 20;				//20 = add bomb
					else
						turn.special = 21;				//21 = add overwriteS
					repu::execTurn(games[games.size() - 1], games[games.size() - 1].getPlayer(playerToCalculate), turn, short(turn.special - 19));

				}
				else
				{
					repu::execTurn(games[games.size() - 1], games[games.size() - 1].getPlayer(playerToCalculate), turn);
				}

			}

			if (!noSort){
				if (depth % game.getPlayers().size() == 0)SortGame(games, fullTurns, player, true);
				else SortGame(games, fullTurns, player, false);
			}


			for (auto& turn : fullTurns) {

				games[counter];
				tempv = 0;
				if (depth % games[counter].getPlayers().size() == 0) {

					tempv = MiniMax(games[counter], player, maxDepth, m_depth, wertung);


					if (cutFlag){
						cutFlag = false;
						break;
					}

					
					if (tempv > wertung.value){													//aktualisierungen falls nötig
						
						wertung.value = tempv;
					}
					if (wertung.alpha < wertung.value){
						
						wertung.alpha = wertung.value;											//\aktualisierungen falls nötig
					}

					if (tempv >= wertung.beta){													//cutoffbedingung prüfen
						
						cutFlag = true;
						return wertung.value;													//\cutoffbedingung prüfen
					}
					
					
				}
				else {

					tempv = MiniMax(games[counter], player, maxDepth, m_depth, wertung);
					if (cutFlag){
						cutFlag = false;
						break;
					}
				
					if (tempv < wertung.value){													//aktualisierungen falls nötig
					
						wertung.value = tempv;
					}
					if (wertung.beta > wertung.value){
						
						wertung.beta = wertung.value;											//\aktualisierungen falls nötig
					}
					if (tempv <= wertung.alpha)	{												//cutoffbedingung prüfen
						
						cutFlag = true;
						return wertung.value;													//\cutoffbedingung prüfen
					}
					
				}
				counter++;
			}



		}
		return wertung.value;
	}


	ff::turnPos AlgorithmBase::MiniMaxPos(ff::Spiel& game, short player, int maxDepth) {		//minimax gibt die position zurück
		run = true;
		game.getPlayer(player).clearValidTurnswoX();						//cleard überschreib und normale züge
		ff::startTurns(game, player);

		std::vector<ff::turnPos> turns = game.getPlayer(player).getAllowedTurns();

		int overwriteDivisor = 2;


		std::vector<ff::turnPos> tempTurns;
		if (game.getPlayer(player).getAllowedOverwriteTurns().size() > 0 && ((turns.size() < 5 && game.freeFields() < 10) || turns.size() < 3) && game.getPlayer(player).getNumOverrideStones() > 0) { // TO-DO balance numbers
			tempTurns = game.getPlayer(player).getAllowedOverwriteTurns();
			turns.insert(turns.end(), tempTurns.begin(), tempTurns.end());
		}
		if (game.getPlayer(player).getAllowedOverwriteTurnsX().size() > 0 && ((turns.size() < 5 && game.freeFields() < 10) || turns.size() < 3) && game.getPlayer(player).getNumOverrideStones() > 0) {// TO-DO balance numbers
			tempTurns = game.getPlayer(player).getAllowedOverwriteTurnsX();
			turns.insert(turns.end(), tempTurns.begin(), tempTurns.end());
		}


		unsigned int bestTurn = 0;
		maxValue = INT_MIN;
		bool cutFlag = false;

		wWertung.alpha = aWmin;
		wWertung.beta = aWmax;
		
		int temp = 0;
		zaehler = 0;

	

		//SORT
		std::vector<ff::Spiel> games;
		for (auto& turn : turns){
			games.push_back(game);
			
			if (games[games.size() - 1].getMap()[turn.pos_Y][turn.pos_X].art == ff::Feld::Art::Choice)
			{
				

				short *stones = new short[game.getSpielerzahl()];

				for (unsigned short i = 1; i < game.getSpielerzahl(); i++)
				{
					stones[i - 1] = games[games.size() - 1].getPlayer(i).getPossibleStartPosition().size();
				}

				short biggestValPlayer = 0;

				for (unsigned short i = 1; i < game.getSpielerzahl(); i++)
				{
					if (stones[biggestValPlayer] < stones[i])
						biggestValPlayer = i;
				}

				delete[] stones;

				turn.special = biggestValPlayer + 1;

				repu::execTurn(games[games.size() - 1], games[games.size() - 1].getPlayer(player), turn, char(turn.special + 48));
			}
			else if (games[games.size() - 1].getMap()[turn.pos_Y][turn.pos_X].art == ff::Feld::Art::Bonus)
			{
				
				if (game.getStaerke() > 1)
					turn.special = 20;				//20 = add bomb
				else
					turn.special = 21;				//21 = add overwriteS
				repu::execTurn(games[games.size() - 1], games[games.size() - 1].getPlayer(player), turn, short(turn.special - 19));

			}
			else
			{
				repu::execTurn(games[games.size() - 1], games[games.size() - 1].getPlayer(player), turn);
			}
		}
		if (!noSort)
			SortGame(games, turns, player,true);

	

		for (unsigned int i = 0; i < turns.size(); i++) {
			if (run)
			{
				zaehler++;

				


				if (turns[i].direction.empty() == false)
					temp = MiniMax(games[i], player, maxDepth, 1, wWertung);
				else
					temp = MiniMax(games[i], player, maxDepth, 1, wWertung) / overwriteDivisor;

				
				if (temp == std::numeric_limits<int>::max())
					temp = 0;
				if (maxValue < temp){
					maxValue = temp;
					bestTurn = i;
				}
			}
		}
		tValue = maxValue;
		if (debug & run)
		{
			std::cout << "zugwertung: " << maxValue << std::endl;
			std::cout << "Insgesamte anzahl moeglicher Zuege: " << zaehler << std::endl;
		}

		return turns[bestTurn];

	}

	void AlgorithmBase::MMSearchThread(ff::Spiel game, short player)				//windows
	{
		int offset = 250;
		ff::turnPos Turn;
		
		while (run)
		{
			threadDepth++;
			
			Turn = MiniMaxPos(game, player, threadDepth);


			//faillow
			if (maxValue < aWmin && maxValue != 0)
			{
				if (debug)
					std::cout << "faillow!\n";
				aWmin = INT_MIN;
			}
			else if (maxValue > aWmax)	//failhigh
			{
				if (debug)
					std::cout << "failhigh!\n";
				aWmax = INT_MAX;
			}


			if (maxValue == 0 || maxValue == INT_MIN || maxValue == INT_MAX)
			{
				aWmin = -1 * offset;
				aWmax = offset;
			}
			else  if (maxValue > 0)
			{
				Turn_t = Turn;

				aWmin = -1 * (maxValue + offset);
				aWmax = (maxValue + offset);
			}
			else {
				Turn_t = Turn;

				aWmin = (maxValue + offset);
				aWmax = -1 * (maxValue + offset);
			}


		}


		//reset for next turn...
		aWmax = INT_MAX;
		aWmin = INT_MIN;

	}

	ff::turnPos AlgorithmBase::MiniMaxDeep(ff::Spiel& game, short player, uint32_t time)
	{
		repu::clock timer;
		Turn_t = ff::turnPos{ -1, -1 };
		run = true;

		threadDepth = 0;
		std::thread searchthread(&AlgorithmBase::MMSearchThread, this, game, player);
		searchthread.detach();
		uint32_t offset = 2000;	
		aspDelta = 0;
		int oldDelta = aspDelta;
		int oldDepth = threadDepth;
		uint32_t oldTime = 0;
		int oldValue = 0;
		short abortcounter = 0;
		ff::turnPos bestTurn;
		int maxvalue = 0;

		while (uint32_t(timer.getAsMilSecs()) + offset < time)
		{
			if (threadDepth != oldDepth)
			{
				aspDelta = int(timer.getAsMilSecs() - oldTime);
				if (uint32_t(timer.getAsMilSecs() + oldTime + aspDelta + 20) + offset >= time)
				{
					if (debug)
						std::cout << "cant make next depth in time, stopping!" << std::endl;
					run = false;
				}

				oldDepth = threadDepth;
				oldTime = (uint32_t)timer.getAsMilSecs();
				oldDelta = aspDelta;
				oldValue = tValue;
				if (debug)
					std::cout << "depth: " << threadDepth << " took " << timer.getAsMilSecs() << " millisecs to calculate" << std::endl << std::endl;
			}
			
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (!run)			//if we cant go further stop
				break;
		}
		run = false;
		if (debug)
			std::cout << "managed to get to depth: " << threadDepth << " in " << timer.getAsMilSecs() << "milisecs" << std::endl << std::endl;
		
		if (Turn_t.pos_X == -1 && Turn_t.pos_Y == -1)
		{

			ff::startTurns(game, player);

			if (game.getPlayer(player).getAllowedTurns().size() == 0)
			{
				if (game.getPlayer(player).getAllowedOverwriteTurns().size() == 0)
				{
					ff::turnPos res = game.getPlayer(player).getAllowedOverwriteTurnsX()[0];
					if (game.getMap()[res.pos_Y][res.pos_X].art == ff::Feld::Art::Bonus && res.special == 0)
					{
						res.special = 20; //Bombe

					}
					else if (game.getMap()[res.pos_Y][res.pos_X].art == ff::Feld::Art::Choice && res.special == 0){
						res.special = player;
					}

					return res;
				}
				ff::turnPos res = game.getPlayer(player).getAllowedOverwriteTurns()[0];
				if (game.getMap()[res.pos_Y][res.pos_X].art == ff::Feld::Art::Bonus && res.special == 0)
				{
					res.special = 20; //Bombe

				}
				else if (game.getMap()[res.pos_Y][res.pos_X].art == ff::Feld::Art::Choice && res.special == 0){
					res.special = player;
				}

				return res;
			}
			ff::turnPos res = game.getPlayer(player).getAllowedTurns()[0];
			if (game.getMap()[res.pos_Y][res.pos_X].art == ff::Feld::Art::Bonus && res.special == 0)
			{
				res.special = 20; //Bombe

			}
			else if (game.getMap()[res.pos_Y][res.pos_X].art == ff::Feld::Art::Choice && res.special == 0){
				res.special = player;
			}

			return res;
		}
		if (game.getMap()[Turn_t.pos_Y][Turn_t.pos_X].art == ff::Feld::Art::Bonus && Turn_t.special == 0)
		{
			Turn_t.special = 20; //Bombe

		}
		else if (game.getMap()[Turn_t.pos_Y][Turn_t.pos_X].art == ff::Feld::Art::Choice && Turn_t.special == 0){
			Turn_t.special = player;
		}
		return Turn_t;
	}

	ff::turnPos AlgorithmBase::bombPos(ff::Spiel& G, int myPlayerNumber){

		ff::turnPos pos;													//Die zu übergende Position						
		int EnemyBestValue = INT_MAX;										//Der niedrigeste Wert den wir unserm gegner zuweißen können
		int EnemyNextValue = 0;												//Ein temporärer Wert der immer den Wert unseres Gegners für das die Derzeitige bombe beträgt
		int myValue = 0;													//Hält unsere Punktzahl
		int diffValue = INT_MIN;											//Differenz aus unseren punkten und denen des Gegners
		int scariestPlayer;													//gegner mit dem Platz vor uns
		ff::Spiel Spiel;													// temporäres spiel
		std::vector<rank> board = ms::Evaluator::getRanking(G);				//Ranking für gegner bestimmen										
		std::sort(board.begin(), board.end(), CompareScore);				//Ranking sortieren

		if (board.at(0).player == myPlayerNumber){							//Sind wir an erster Stelle, minimieren wir den zweiten noch mehr
			scariestPlayer = board.at(1).player;
		}
		else{																//anderenfalls suchen wir nach dem gegner über uns
			for (int i = 0; i < G.getSpielerzahl(); i++){
				if (board.at(i).player == myPlayerNumber)
					scariestPlayer = board.at(i - 1).player;
			}
		}

		for (int x = 0; x < G.getBreite(); x++){									//Ausführen von bomben für alle felder
			for (int y = 0; y < G.getHoehe(); y++){

				Spiel = G;

				if (Spiel.getFeld(x, y).art != ff::Feld::Art::Hole){				//Wenn kein leeres Feld ausführen
					repu::execBomb(Spiel, repu::bombStep{ G.getStaerke(), ff::turnPos{ x, y } });
					

					EnemyNextValue = 0;
					myValue = 0;
					for (int yy = 0; yy < G.getHoehe(); yy++){								//zählen der steine für uns und dem gegner über uns
						for (int xx = 0; xx < G.getBreite(); xx++){

							if (Spiel.getFeld(xx, yy).art == static_cast<char>(scariestPlayer)+48)
								EnemyNextValue++;
							if (Spiel.getFeld(xx, yy).art == static_cast<char>(myPlayerNumber)+48)
								myValue++;
						}
					}

					if (diffValue < myValue - EnemyNextValue){						//und wir gleichzeitig nicht uns viele gelöscht haben 
						diffValue = myValue - EnemyNextValue;

						if (EnemyBestValue >= EnemyNextValue){								//wenn Wir unseren gegner noch mehr punkte nehmen konnten oder gleichgeblieben ist
							EnemyBestValue = EnemyNextValue;								//wird das zur geringsten value
							pos = ff::turnPos{ x, y };									// neuester Bester bombspot
						}
					}
				}
			}
		}

		return pos;
	}

}