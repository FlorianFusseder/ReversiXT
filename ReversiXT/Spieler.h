#ifndef SPIELER_H
#define SPIELER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace ff{

	struct turnPos{
		int pos_X;
		int pos_Y;
		std::vector<short> direction;
		short special;
	};


	class Spieler{

		short playerNumber;
		char playerChar;
		unsigned short numberOfBombs;
		unsigned short numberOfOverwritestone;
		std::vector<turnPos> possibleStartPosition;
		std::vector<turnPos> allowedTurn;
		std::vector<turnPos> allowedOverwriteTurnX;
		std::vector<turnPos> allowedOverwriteTurn;

	public:
		Spieler() : Spieler('0', 0, 0, possibleStartPosition, allowedTurn, allowedOverwriteTurn, allowedOverwriteTurnX){}
		Spieler(char PlayerChar, unsigned short NumberOfBombs, unsigned short NumberOfOverwritestone);
		Spieler(char playerChar, unsigned short numberOfBombs, unsigned short numberOfOverwritestone, std::vector<turnPos> possibleStartPosition, std::vector<turnPos> allowedTurn, std::vector<turnPos> allowedOverwriteTurn, std::vector<turnPos> allowedOverwriteTurnX);
		~Spieler();

		auto addBomb() -> void;
		auto addOverrideS() -> void;
		auto removeBomb() -> void;
		auto removeOverrideS() -> void;
		auto clearOverrideS() -> void;

		auto is_allowed(int, int, short) -> bool;


		auto addAllowedTurn(int, int, short) -> void;

		auto addPossibleStartPosition(int, int) -> void;
		auto removePossibleStartPosition(int, int) -> void;

		auto addAllowedOverwriteTurn(int, int, short) -> void;

		auto addAllowedOverwriteTurnX(int, int, short) -> void;

		auto clearValidTurns() -> void;
		auto clearValidTurnswoX() -> void;

		auto clearStartPos() -> void;

		auto getPlayerNumber() -> short;

		auto getPlayerChar() -> char;

		auto getNumOverrideStones() -> unsigned short;

		auto getAllowedTurns() -> std::vector < turnPos > ;

		auto getPossibleStartPosition()->std::vector < turnPos >;

		auto getAllowedOverwriteTurns()->std::vector < turnPos > ;
		
		auto getAllowedOverwriteTurnsX()->std::vector < turnPos > ;

		auto outputData() -> void;

	};
}
#endif