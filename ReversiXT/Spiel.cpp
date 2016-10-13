#include "Spiel.h"

namespace ff{

	

	Spiel::Spiel(unsigned short s, unsigned short u, unsigned short b, unsigned short str, unsigned short bh, unsigned short bb, unsigned short m, std::vector<std::vector<ff::Feld>> myvec, std::vector<ff::Spieler> sp) : spielerzahl(s), ueberschreibsteine(u), bomben(b), staerke(str), bretthoehe(bh), brettbreite(bb) ,multiplikator(m),map(myvec), players(sp){}


	Spiel::~Spiel(){}


	auto Spiel::einlesen(std::stringstream& mapfile) -> int
	{
		char tempchar;

		//read basic information
		mapfile >> spielerzahl >> ueberschreibsteine >> bomben >> staerke >> bretthoehe >> brettbreite;	

		for (unsigned short i = 1; i < spielerzahl + 1; i++)
		{
			ff::Spieler p(static_cast<char>(i)+48, bomben, ueberschreibsteine);
			players.push_back(p);
			activePlayers.flip(i - 1);
		}

		///// Read playingfields
		for (int y = 0; y < bretthoehe; y++)
		{
			std::vector<Feld> vec_X;
			for (int x = 0; x < brettbreite; x++)
			{
				mapfile >> tempchar;
				vec_X.push_back(Feld{ tempchar });
				vec_X.back().score += ff::Feld::Wert::Normal;
				if (tempchar == Feld::Art::Empty || tempchar == Feld::Art::Bonus || tempchar == Feld::Art::Choice || tempchar == Feld::Art::Inversion){
					_freeFields++;
				}
				if (tempchar >= '1' && tempchar <= '8'){
					vec_X.back().playerField = true;
					players.at(static_cast<int>(tempchar)-49).addPossibleStartPosition(x, y);
					
				}
				if (tempchar == 'x'){
					vec_X.back().playerField = true;												//Wird wie Fremder Spieler behandelt!!!		
					for (auto &p : players){
						p.addAllowedOverwriteTurnX(x, y, -1);
					}
				}

			}
			map.push_back(vec_X);
		}

		multiplikator = _freeFields / 100 +1 ;

		/// \Read playingfields
		//  Wertung felder neben guten feldern

		for (int y = 0; y < bretthoehe; y++)
		{
			for (int x = 0; x < brettbreite; x++)
			{
				tempchar = map[y][x].art;

				if (tempchar == Feld::Art::Empty || tempchar == Feld::Art::Bonus || tempchar == Feld::Art::Choice || tempchar == Feld::Art::Inversion){
						
					if (tempchar == Feld::Art::Bonus)
						Rate(x, y, ff::Feld::Wert::BonusW, ff::Feld::Wert::NebenBonus);
						
						
					if (tempchar == Feld::Art::Choice)
						Rate(x, y, ff::Feld::Wert::ChoiceW, ff::Feld::Wert::NebenChoice);
						
					if (tempchar == Feld::Art::Inversion)
						Rate(x, y, ff::Feld::Wert::Inversition, ff::Feld::Wert::NebenInversition);
				}
			}
		}
		//\Wertung felder neben guten feldern

		//Find Corners
		

		// read transitions
		while (mapfile)
		{

			short transarr[6];
			char throwaway;

			mapfile >> transarr[0] >> transarr[1] >> transarr[2] >> throwaway >> throwaway >> throwaway >> transarr[3] >> transarr[4] >> transarr[5];

			if (!mapfile) // == eof : no transitions
			{
				

				for (int y = 0; y < bretthoehe; y++)
				{
					for (int x = 0; x < brettbreite; x++)
					{
						tempchar = map[y][x].art;

						if (tempchar == Feld::Art::Bonus || tempchar == Feld::Art::Choice || tempchar == Feld::Art::Inversion){

							if (tempchar == Feld::Art::Bonus)
								for (auto it = map[y][x].Transition.begin(); it != map[y][x].Transition.end(); it++){	//transition vorhanden?
								map[it->second.turnPos.pos_Y][it->second.turnPos.pos_X].score += ff::Feld::Wert::NebenBonus* multiplikator;			//transitionsgegenstück suchen
								}


							if (tempchar == Feld::Art::Choice)
								for (auto it = map[y][x].Transition.begin(); it != map[y][x].Transition.end(); it++){	//transition vorhanden?
								map[it->second.turnPos.pos_Y][it->second.turnPos.pos_X].score += ff::Feld::Wert::NebenChoice* multiplikator;			//transitionsgegenstück suchen
								}

							if (tempchar == Feld::Art::Inversion)
								for (auto it = map[y][x].Transition.begin(); it != map[y][x].Transition.end(); it++){	//transition vorhanden?
								map[it->second.turnPos.pos_Y][it->second.turnPos.pos_X].score += ff::Feld::Wert::NebenInversition* multiplikator;			//transitionsgegenstück suchen
								}
						}
					}
				}

				ms::ScoreHelper::findEdges(map, multiplikator);

				return 1;
			}

			if (!(transarr[0] == transarr[3] && transarr[1] == transarr[4])){
				map[transarr[1]][transarr[0]].Transition.emplace(transarr[2], transData{ turnPos{ transarr[3], transarr[4] }, transarr[5] });
				map[transarr[4]][transarr[3]].Transition.emplace(transarr[5], transData{ turnPos{ transarr[0], transarr[1] }, transarr[2] });
			}

		}


		ms::ScoreHelper::findEdges(map, multiplikator);
		return 1;
	}

	void Spiel::Rate(int x, int y, int wert, int minuswert){

		map[y][x].score += wert * multiplikator;

		for (int addy = -1; addy <= 1; addy++)															//außenrum (richtungen quasi)...
			for (int addx = -1; addx <= 1; addx++)														//...werte für felder
				if (!(addx == 0 && addy == 0))															// außer 0, 0 -> is ja genau das feld selbst
					if (x + addx >= 0 && y + addy >= 0 && x + addx <= brettbreite - 1 && y + addy <= bretthoehe - 1)			//wenn nich out of bounds und kein leeres feld
						if (map[y + addy][x + addx].art != ff::Feld::Art::Hole)
							map[y + addy][x + addx].score += minuswert;										//bewertung vor feld

	}


	auto Spiel::outputData() -> void {

		std::cout << "Spielerzahl: " << spielerzahl << std::endl
			<< "Überschreibsteine: " << ueberschreibsteine << std::endl
			<< "Bomben: " << bomben << std::endl
			<< "Stärke: " << staerke << std::endl
			<< "Bretthöhe: " << bretthoehe << std::endl
			<< L"Brettbreite: " << brettbreite << std::endl;
		return;
	}

	auto Spiel::outputBrett() -> void{

		int b = brettbreite;
		int h = bretthoehe;

		for (int y = 0; y < h; y++){
			for (int x = 0; x < b; x++){
				std::cout << map[y][x].art; 
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

	}

	void Spiel::outputBrettVal(){

		int b = brettbreite;
		int h = bretthoehe;

		for (int y = 0; y < h; y++){
			for (int x = 0; x < b; x++){
				std::cout << map[y][x].score << " "; 
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	auto Spiel::outputLegitTurns() -> void{

		for (auto p : players){
			std::cout << "Spieler " << p.getPlayerNumber() << " char " << p.getPlayerChar() << std::endl;
			std::cout << p.getAllowedTurns().size() << std::endl;
			for (auto t : p.getAllowedTurns()){
				std::cout << "X: " << t.pos_X << "      " << "Y: " << t.pos_Y  << " R:";
				for (auto r : t.direction)
					std::cout << " " << r;
				std::cout << std::endl;
			}

			std::cout << "over" << std::endl;
			for (auto s : p.getAllowedOverwriteTurns()){
				std::cout << "X: " << s.pos_X << "      " << "Y: " << s.pos_Y << std::endl;
			}
			std::cout << "X" << std::endl;
			for (auto v : p.getAllowedOverwriteTurnsX())
				std::cout << "X: " << v.pos_X << "      " << "Y: " << v.pos_Y << std::endl;

			std::cout << std::endl;
			std::cout << "Gesamt " << p.getAllowedOverwriteTurns().size() + p.getAllowedOverwriteTurnsX().size() + p.getAllowedTurns().size() << std::endl;;
		}
		
	
	}

	auto Spiel::outputLegitOverwrites() -> void{

		for (auto p : players){
			std::cout << "Spieler " << p.getPlayerNumber() << " char " << p.getPlayerChar() << std::endl;
			std::cout << p.getAllowedOverwriteTurns().size() << std::endl;
			for (auto t : p.getAllowedOverwriteTurns()){
				std::cout << "X: " << t.pos_X << "      " << "Y: " << t.pos_Y << std::endl;
			}
		}
	}

	auto Spiel::getSpielerzahl() -> unsigned short{
		return spielerzahl;
	}

	auto Spiel::getUeberschreibsteine() -> unsigned short{
		return ueberschreibsteine;
	}

	auto Spiel::getBomben() -> unsigned short{
		return bomben;
	}

	auto Spiel::getStaerke() -> unsigned short{
		return staerke;
	}

	auto Spiel::getBreite() -> unsigned short{
		return brettbreite;
	}

	auto Spiel::getHoehe() -> unsigned short{
		return bretthoehe;
	}

	std::vector<std::vector<Feld>>&  Spiel::getMap(){
		return map;
	}

	ff::Feld& Spiel::getFeld(int x, int y){
		try{
			return map.at(y).at(x);
		}
		catch (const std::out_of_range& oor){
			throw oor;
		}
	}

	std::vector<ff::Spieler> & Spiel::getPlayers(){
		return players;
	}

	auto ff::Spiel::getMul() -> unsigned short{
		return multiplikator;
	}

	ff::Spieler& Spiel::getPlayer(int number){
		return players.at(number - 1);
	}

	ff::Spieler& Spiel::getPlayer(char playerchar){
		return players.at(static_cast<int>(playerchar)-49);
	}

	auto Spiel::setSpielerzahl(unsigned short x) -> void{
		spielerzahl = x;
		return;
	}

	auto Spiel::setUeberschreibsteine(unsigned short x) -> void{
		ueberschreibsteine = x;
		return;
	}

	auto Spiel::setBomben(unsigned short x) -> void{
		bomben = x;
		return;
	}

	auto Spiel::setStaerke(unsigned short x) -> void{
		staerke = x;
		return;
	}

	auto Spiel::setBretthoehe(unsigned short x) -> void{
		bretthoehe = x;
		return;
	}

	auto Spiel::setBrettbreite(unsigned short x) -> void{
		brettbreite = x;
		return;
	}

	auto Spiel::setLayout(int y, int x, char c) -> void{

		if (map.empty()){
			std::vector<ff::Feld> temp(brettbreite);
			for (int i = 0; i < bretthoehe; i++){
				map.push_back(temp);
			}

		}
		ff::Feld tempfeld{ c };
		map[y][x] = tempfeld;
		return;
	}

	auto Spiel::addAllowedOverwrite(char c, int x, int y, short d) -> void{
		players.at(static_cast<int>(c)-49).addAllowedOverwriteTurn(x, y, d);
		return;
	}

	auto Spiel::addAllowedTurn(char c, int x, int y, short d) -> void{
		players.at(static_cast<int>(c)-49).addAllowedTurn(x, y, d);
		return;
	}

	void Spiel::minusFreeFields()
	{
		_freeFields--;
	}
	unsigned int Spiel::freeFields()
	{
		return _freeFields;
	}
}