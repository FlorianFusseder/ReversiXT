#include "ExecTurn.h"



namespace repu
{
	//set and X, Y modifiers for direction (i)
	void setXYj(int & xj, int & yj, short i)						
	{
		///X
		if (i > 0 && i < 4)
		{
			xj = 1;
		}
		else if (i > 4)
		{
			xj = -1;
		}
		else {
			xj = 0;
		}
		///Y
		if (i > 2 && i < 6)
		{
			yj = 1;
		}
		else if (i == 0 || i == 1 || i == 7)
		{
			yj = -1;
		}
		else
		{
			yj = 0;
		}
	}

	short reverseDir(short dir)														// transitionen halten immer die AUSGANGS-Richtung
	{																				// also, reverse fürs reingehen 
		short rDir = dir + 4;
		if (rDir > 7) rDir = rDir - 8;
		return rDir;
	}

	//use transition
	void useTrans(ff::Spiel& game, short &dir, int &X, int &Y, char playerchar, std::vector<ff::turnPos>& writeS)		
	{

		std::vector<std::vector<ff::Feld>>& map = game.getMap();

		int oldX = X;																							//needed to check for correct output dir
		int oldY = Y;																							//needed to check for correct output dir

		short oldDir = dir;

		ff::transData transpartner = map[Y][X].Transition.find(dir)->second;										//set new X and Y //map kein vector... also nicht .at(..) sondern find
		X = transpartner.turnPos.pos_X;
		Y = transpartner.turnPos.pos_Y;


		if (map[Y][X].art != playerchar)
		{																		//set playerstone
			writeS.push_back(ff::turnPos{ X, Y });
		}

		for (auto& i : map[Y][X].Transition)																	//check new fields transitions to get new direction
		{
			if (i.second.turnPos.pos_X == oldX && i.second.turnPos.pos_Y == oldY)
			{																								//if transition links back to old field
				dir = reverseDir(i.first);																		//we have our new direction
				if (dir != reverseDir(oldDir))
					break;
			}
		}
	}

	int execOverride(ff::Spiel & game, ff::turnPos coords, char playerchar)
	{
		std::vector<std::vector<ff::Feld>>& map = game.getMap();
		int X;																									//coords
		int Y;
		int xj;																									//coord modifiers
		int yj;
		bool first;

		std::vector<ff::turnPos> writeS[8];																		//yip, one vector for every dir

		X = coords.pos_X;																				//go to starting position	
		Y = coords.pos_Y;
		bool breakouter;

		for (short odir = 0, dir = 0; odir < 8; odir++, dir++)
		{
			
			X = coords.pos_X;																				//go to starting position	
			Y = coords.pos_Y;																	//set direction
			dir = odir;
			setXYj(xj, yj, dir);																			//calculate X and Y modifiers
			first = true;
			writeS[odir].push_back(ff::turnPos{ X, Y });

			while (map[Y][X].art != playerchar || first == true)																						//as long as we dont hit our own field, we colour our stones
			{

				breakouter = false;
				writeS[odir].push_back(ff::turnPos{ X, Y });

				while (map[Y][X].Transition.count(dir)>0)											//if we hit a field with transition, in our direction
				{																							//dir is reversed since transitions hold the OUTGOING dir		
					int oldX = X;																							//needed to check for correct output dir
					int oldY = Y;
					short oldDir = dir;

					ff::turnPos transpartner = map[Y][X].Transition.at(dir).turnPos;												//set new X and Y
					X = transpartner.pos_X;
					Y = transpartner.pos_Y;


					if (map[Y][X].art != playerchar)
					{	
						if (map[Y][X].playerField == false || (coords.pos_X == X && coords.pos_Y ==Y))
						{
							writeS[odir].clear();
							breakouter = true;
							break;
						}
	
						writeS[odir].push_back(ff::turnPos{ X, Y });
					}
					if (map[Y][X].art == playerchar)
					{
						breakouter = true;
						break;
					}
					for (auto& i : map[Y][X].Transition)																	//check new fields transitions to get new direction
					{
						if (i.second.turnPos.pos_X == oldX && i.second.turnPos.pos_Y == oldY)
						{																								//if transition links back to old field
							dir = reverseDir(i.first);																		//we have our new direction
							if (dir != reverseDir(oldDir))
								break;
						}
					}											//we "use" the transition....

					setXYj(xj, yj, dir);																	//recalc modifiers with new dir from transition

				}
				if (breakouter)
					break;

				if (map[Y][X].art != playerchar || first == true)
				{
					X = X + xj;
					Y = Y + yj;
				}

				first = false;
				if (X < 0 || X >= game.getBreite() || Y < 0 || Y >= game.getHoehe() 
					|| map[Y][X].playerField == false || X == coords.pos_X && Y == coords.pos_Y)
				{
					writeS[odir].clear();
					break;
				}

			}

		}

		if (map[coords.pos_Y][coords.pos_X].art == ff::Feld::Art::Expansion)
		{
			map[coords.pos_Y][coords.pos_X].art = playerchar;
			map[coords.pos_Y][coords.pos_X].playerField = true;
		}

		for (short dir = 0; dir < 8; dir++)
		{
			for (auto i : writeS[dir])
			{
				map[i.pos_Y][i.pos_X].art = playerchar;
				map[i.pos_Y][i.pos_X].playerField = true;
			}
		}
		return 1;

	}

	int exec(ff::Spiel & game, ff::turnPos coords, char playerchar)
	{

		if (coords.direction.empty())
		{
			return execOverride(game, coords, playerchar);
		}

		std::vector<std::vector<ff::Feld>>& map = game.getMap();

		int X;																									//coords
		int Y;
		int xj;																									//coord modifiers
		int yj;

		short dir;																								//direction to go
		bool first;

		std::vector<ff::turnPos> writeS;

		X = coords.pos_X;																				//go to starting position	
		Y = coords.pos_Y;
		
		writeS.push_back(ff::turnPos{ X, Y });


		for (unsigned int k = 0; k < coords.direction.size(); k++)
		{
				X = coords.pos_X;																				//go to starting position	
				Y = coords.pos_Y;																					
				dir = coords.direction[k];																		//set direction
				setXYj(xj, yj, dir);																			//calculate X and Y modifiers

				first = true;
				writeS.push_back(ff::turnPos{ X,Y });
																							//placeholder, since we may go to start multiple times
				while (map[Y][X].art != playerchar || first == true/* && map[Y][X].art != '%'*/)																						//as long as we dont hit our own field, we colour our stones
				{

					writeS.push_back(ff::turnPos{ X, Y });

					while (map[Y][X].Transition.count(dir)>0 && map[Y][X].art != playerchar)											//if we hit a field with transition, in our direction
					{																							//dir is reversed since transitions hold the OUTGOING dir		

						useTrans(game, dir, X, Y, playerchar, writeS);													//we "use" the transition....

						if (map[Y][X].art == playerchar){
							first = false;
							break;
						}

						setXYj(xj, yj, dir);																	//recalc modifiers with new dir from transition

					} 
					if (map[Y][X].art != playerchar || first == true)
					{
						X = X + xj;
						Y = Y + yj;
					}

					first = false;
				}			
		}

		for (auto i : writeS)
		{
			map[i.pos_Y][i.pos_X].art = playerchar;
			map[i.pos_Y][i.pos_X].playerField = true;
		}
		return 0;
	}

	void specialChoice(ff::Spiel& Game, ff::Spieler& Player, char choice)
	{
		std::vector<std::vector<ff::Feld>> &map = Game.getMap();
		char playerchar = Player.getPlayerChar();

		std::vector<ff::turnPos> writeS;

		for (unsigned int y = 0; y < map.size(); y++)
		{

			for (unsigned int x = 0; x < map[0].size(); x++)
			{
				if (map[y][x].art == choice)
					writeS.push_back(ff::turnPos{ (int)x,(int) y });

				if (map[y][x].art == playerchar)
				{
					map[y][x].art = choice;
				}
			}
		}

		for (auto i : writeS)
		{
			map[i.pos_Y][i.pos_X].art = playerchar;
			map[i.pos_Y][i.pos_X].playerField = true;
		}
	}

	void specialBonus(ff::Spieler& Player, short bonus)
	{
		switch (bonus)
		{
		case 1:
			Player.addBomb();
			break;
		case 2:
			Player.addOverrideS();
			break;
		}
	}

	void specialInverse(ff::Spiel& Game)
	{
		std::vector<std::vector<ff::Feld>> &map = Game.getMap();
		short numPlayers = Game.getSpielerzahl();

		for (auto& y : map)
		{

			for (auto& x : y)
			{
				if (x.playerField && x.art != ff::Feld::Art::Expansion)
				{
					x.art++;
					if (x.art > numPlayers+48)
					{
						x.art -= numPlayers;
					}
				}
			}
		}
	}

	void newStartPos(ff::Spiel& Game)
	{
		std::vector<std::vector<ff::Feld>>& map = Game.getMap();

		char tempchar;


		std::vector<ff::Spieler> & players = Game.getPlayers();

		for (auto & p : players)
		{
			p.clearStartPos();
		}

		///// Read playingfields
		for (int y = 0; y < Game.getHoehe(); y++)
		{
			for (int x = 0; x < Game.getBreite(); x++)
			{
				tempchar = map[y][x].art;
				if (tempchar >= '1' && tempchar <= '8'){
								Game.getPlayer(tempchar).addPossibleStartPosition(x, y);
				}
				if (tempchar == 'x'){
					map[y][x].playerField = true;												//Wird wie Fremder Spieler behandelt!!!
					for (auto &p : players){
						p.addAllowedOverwriteTurnX(x, y, -1);
					}
				}

			}
		}

	}

	void execTurn(ff::Spiel& Game, ff::Spieler& Player, ff::turnPos coords, char choice, short bonus)
	{
		std::vector<std::vector<ff::Feld>> &map = Game.getMap();
		char playerchar = Player.getPlayerChar();

		char special = map[coords.pos_Y][coords.pos_X].art;

		if (map[coords.pos_Y][coords.pos_X].playerField)
		{
			Player.removeOverrideS();
		}

		switch (special)
		{
		case ff::Feld::Art::Choice:
			if (choice == -1)
			{
				choice = Player.getPlayerChar();
		
			}
			break;
		case ff::Feld::Art::Bonus:
			if (bonus == -1)
			{
				bonus = 1;						
				
			}
		}

		int xmax = map.at(0).size() - 1;
		int ymax = map.size() - 1;

		for (auto it = map[coords.pos_Y][coords.pos_X].Transition.begin(); it != map[coords.pos_Y][coords.pos_X].Transition.end(); it++){	//transition vorhanden?
			if (map[coords.pos_Y][coords.pos_X].art == ff::Feld::Art::Bonus)
				map[it->second.turnPos.pos_Y][it->second.turnPos.pos_X].score -= ff::Feld::Wert::NebenBonus * Game.getMul();
			if (map[coords.pos_Y][coords.pos_X].art == ff::Feld::Art::Inversion)
				map[it->second.turnPos.pos_Y][it->second.turnPos.pos_X].score -= ff::Feld::Wert::NebenInversition* Game.getMul();
			if (map[coords.pos_Y][coords.pos_X].art == ff::Feld::Art::Choice)
				map[it->second.turnPos.pos_Y][it->second.turnPos.pos_X].score -= ff::Feld::Wert::NebenChoice* Game.getMul();
		}

		for (int addy = -1; addy <= 1; addy++){															//außenrum (richtungen quasi)...
			for (int addx = -1; addx <= 1; addx++)														//...werte für felder
				if (!(addx == 0 && addy == 0))															// außer 0, 0 -> is ja genau das feld selbst
					if (coords.pos_X + addx >= 0 && coords.pos_Y + addy >= 0 && coords.pos_X + addx <= xmax && coords.pos_Y + addy <= ymax && map[coords.pos_Y + addy][coords.pos_X + addx].art != ff::Feld::Art::Hole){			//wenn nich out of bounds und kein leeres feld

						if (map[coords.pos_Y][coords.pos_X].art == ff::Feld::Art::Bonus)
							map[coords.pos_Y + addy][coords.pos_X + addx].score -= ff::Feld::Wert::NebenBonus* Game.getMul();
						if (map[coords.pos_Y][coords.pos_X].art == ff::Feld::Art::Inversion)
							map[coords.pos_Y + addy][coords.pos_X + addx].score -= ff::Feld::Wert::NebenInversition* Game.getMul();
						if (map[coords.pos_Y][coords.pos_X].art == ff::Feld::Art::Choice)
							map[coords.pos_Y + addy][coords.pos_X + addx].score -= ff::Feld::Wert::NebenChoice* Game.getMul();

					}


		}

		if (map[coords.pos_Y][coords.pos_X].art == ff::Feld::Art::Bonus)
			map[coords.pos_Y][coords.pos_X].score -= ff::Feld::Wert::BonusW* Game.getMul();
		if (map[coords.pos_Y][coords.pos_X].art == ff::Feld::Art::Inversion)
			map[coords.pos_Y][coords.pos_X].score -= ff::Feld::Wert::Inversition* Game.getMul();
		if (map[coords.pos_Y][coords.pos_X].art == ff::Feld::Art::Choice)
			map[coords.pos_Y][coords.pos_X].score -= ff::Feld::Wert::ChoiceW* Game.getMul();

		
		exec(Game, coords, playerchar);

		switch (special)
		{
		case ff::Feld::Art::Choice:
			specialChoice(Game, Player, choice);
			//newStartPos(Game);
			break;
		case ff::Feld::Art::Bonus:
			specialBonus(Player, bonus);
			break;
		case ff::Feld::Art::Inversion:
			specialInverse(Game);
			//newStartPos(Game);
		}

		//clear all valid turns
		for (auto &player : Game.getPlayers())
		{
			player.clearValidTurns();
		}
		newStartPos(Game);
		Game.minusFreeFields();

	}

	void execTurn(ff::Spiel& Game, ff::Spieler& Player, ff::turnPos coords, char choice)
	{
		execTurn(Game, Player, coords, choice, -1);
	}

	void execTurn(ff::Spiel& Game, ff::Spieler& Player, ff::turnPos coords, short bonus)
	{
		execTurn(Game, Player, coords, -1, bonus);
	}

	void execTurn(ff::Spiel& Game, ff::Spieler& Player, ff::turnPos coords)
	{
		execTurn(Game, Player, coords, -1, -1);
	}

	void execBomb(ff::Spiel& Game, repu::bombStep coords)
	{
		std::vector<std::vector<ff::Feld>>& map = Game.getMap();
		short strenght = coords.step;
		std::queue<repu::bombStep> toDo;							//queue mit den abzuarbeitenden zügen
		std::vector<repu::bombStep> transitionen;					//neue bombenplätze
		toDo.push(coords);											//ersten spot auf queue legen
		repu::bombStep temp;
		int xmax = Game.getBreite()-1;
		int ymax = Game.getHoehe()-1;

		while (!toDo.empty()){
			temp = toDo.front();									//vordesten stein holen
			if (temp.step == 0)
				map[temp.coords.pos_Y][temp.coords.pos_X] = ff::Feld::Art::Hole;	// ist die tempweite 0 Zug ausführen, nichts auf queue legen fertig mit diesem zug
			else{
				for (auto it = map[temp.coords.pos_Y][temp.coords.pos_X].Transition.begin(); it != map[temp.coords.pos_Y][temp.coords.pos_X].Transition.end(); it++){	//transition vorhanden?
					if (map[it->second.turnPos.pos_Y][it->second.turnPos.pos_X].art != ff::Feld::Art::Hole)													//transitionsgegenstück suchen
						transitionen.push_back(bombStep{ temp.step - 1, it->second.turnPos });																//dieses gegenstück in vektor legen
				}

				int x = temp.coords.pos_X;
				int y = temp.coords.pos_Y;

				for (int addy = -1; addy <= 1; addy++)															//außenrum (richtungen quasi)...
					for (int addx = -1; addx <= 1; addx++)														//...werte für felder
						if (!(addx == 0 && addy == 0))															// außer 0, 0 -> is ja genau das feld selbst
							if (x + addx >= 0 && y + addy >= 0 && x + addx <= xmax && y + addy <= ymax && map[y + addy][x + addx].art != ff::Feld::Art::Hole)			//wenn nich out of bounds und kein leeres feld
								toDo.push(repu::bombStep{ temp.step - 1, ff::turnPos{ x + addx, y + addy } });															//zu queue mit -1 schrittweite
			}
			map[temp.coords.pos_Y][temp.coords.pos_X].art = ff::Feld::Art::Hole;																						//auszugehendes feld auf leer setzen
			toDo.pop();																																					// und feld aus queue werfen
		} 

		for (auto t : transitionen){
			if (map[t.coords.pos_Y][t.coords.pos_X].art != ff::Feld::Art::Hole)
				execBomb(Game, t);
		}
		//Game.outputBrett();
	}

}