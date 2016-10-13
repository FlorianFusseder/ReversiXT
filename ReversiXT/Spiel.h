#ifndef SPIEL_H
#define SPIEL_H
#define line __LINE__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <bitset>

#include "Feld.h"
#include "Spieler.h"
#include "ScoreHelper.h"

namespace ff{
	


	class Spiel{

		unsigned int _freeFields = 0;
		unsigned short spielerzahl;
		unsigned short ueberschreibsteine;
		unsigned short bomben;
		unsigned short staerke;
		unsigned short bretthoehe;
		unsigned short brettbreite;
		unsigned short multiplikator;

		std::vector<ff::Spieler> players;
		std::vector<std::vector<Feld>> map;
		

	public:
		Spiel(){}
		Spiel(unsigned short s, unsigned short u, unsigned short b, unsigned short str, unsigned short bh, unsigned short bb, unsigned short m, std::vector<std::vector<Feld>> map, std::vector<Spieler> players);
		~Spiel();

		auto einlesen(std::stringstream&) -> int;

		auto outputBrett() -> void;													//Brettlayout ausgabe
		void outputBrettVal();
		auto outputData() -> void;													//Metadaten wie spielerzahl usw ausgeben
		auto outputLegitTurns()-> void;												//gültigen zuge ausgeben
		auto outputLegitOverwrites() -> void;										//gültige Overwritezuüge ausgeben

		auto getSpielerzahl() -> unsigned short;				
		auto getUeberschreibsteine() -> unsigned short;
		auto getBomben() -> unsigned short;
		auto getStaerke() -> unsigned short;
		auto getBreite() -> unsigned short;
		auto getHoehe() -> unsigned short;
		auto getMul() ->  unsigned short;
		ff::Feld& getFeld(int, int);
		std::vector<std::vector<Feld>>& getMap();
		std::vector<ff::Spieler> & getPlayers();	

		ff::Spieler& getPlayer(int);
		ff::Spieler& getPlayer(char playerchar);
		auto setSpielerzahl(unsigned short) -> void;
		auto setUeberschreibsteine(unsigned short) -> void;
		auto setBomben(unsigned short) -> void;
		auto setStaerke(unsigned short) -> void;
		auto setBretthoehe(unsigned short) -> void;
		auto setBrettbreite(unsigned short) -> void;
		auto setLayout(int, int, char) -> void;

		auto delPossibleStartDirection(int, int, short) -> void;
		auto addAllowedTurn(char, int, int, short) -> void;
		auto addAllowedOverwrite(char, int, int, short) -> void;
							

		std::bitset<8> activePlayers;

		void minusFreeFields();
		unsigned int freeFields();
		void Rate(int, int, int, int);
	};

}
#endif
