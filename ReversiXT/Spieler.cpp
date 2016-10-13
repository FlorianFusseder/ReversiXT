#include "Spieler.h"

namespace ff{
	Spieler::Spieler(char PlayerChar, unsigned short NumberOfBombs, unsigned short NumberOfOverwritestone)
	{
		playerChar = PlayerChar;
		numberOfBombs = NumberOfBombs;
		numberOfOverwritestone = NumberOfOverwritestone;
		playerNumber = static_cast<short>(playerChar)-48;
	}
	Spieler::Spieler(char _playerchar, unsigned short _numberOfBomb, unsigned short _numberOfOverwritestone,  std::vector<turnPos> _possibleStartPosition,
		std::vector<turnPos> _allowedTurn, std::vector<turnPos> _allowedOverwriteTurn, std::vector<turnPos> _allowedOverwriteTurnX) : playerChar(_playerchar), numberOfBombs(_numberOfBomb),
		numberOfOverwritestone(_numberOfOverwritestone),  possibleStartPosition(_possibleStartPosition), allowedTurn(_allowedTurn), allowedOverwriteTurn(_allowedOverwriteTurn),
		allowedOverwriteTurnX(_allowedOverwriteTurnX){

			playerNumber = static_cast<short>(playerChar)-48;
	}
	Spieler::~Spieler(){};

	auto Spieler::addBomb() -> void
	{
		numberOfBombs++;
	}

	auto Spieler::addOverrideS() -> void
	{
		numberOfOverwritestone++;
	}

	auto Spieler::removeBomb() -> void
	{
		numberOfBombs--;
	}
	auto Spieler::removeOverrideS() -> void
	{
		numberOfOverwritestone--;
	}

	auto Spieler::clearOverrideS() ->void
	{
		numberOfOverwritestone = 0;
	}

	auto Spieler::is_allowed(int posX, int posY, short d) -> bool{

		for (auto &turn : allowedTurn){
			if (turn.pos_X == posX && turn.pos_Y == posY){
				return true;
			}
		}

		for (auto &turn : allowedOverwriteTurn){
			if (turn.pos_X == posX && turn.pos_Y == posY){
				return true;
			}
		}

		return false;
	}

	auto Spieler::addAllowedTurn(int x, int y, short d) -> void{
		for (auto& i : allowedTurn)
		{
			if (i.pos_X == x && i.pos_Y == y)
			{
				for (auto& k : i.direction)
				{
					if (k != d)
					{
						i.direction.push_back(d);
						return;
					}
				}
			}
		}
		allowedTurn.push_back(turnPos{ x, y });
		allowedTurn.back().direction.push_back(d);
	}

	auto Spieler::addPossibleStartPosition(int x, int y) -> void{
		for (auto& i : possibleStartPosition)
		{
			if (i.pos_X == x && i.pos_Y == y)
			{
				return;
			}
		}
		possibleStartPosition.push_back(turnPos{ x, y });
	}

	auto Spieler::removePossibleStartPosition(int x, int y) -> void{
		bool found = false;
		unsigned int i;
		for (i = 0; i < possibleStartPosition.size(); i++)
		{
			if (possibleStartPosition[i].pos_X == x && possibleStartPosition[i].pos_Y == y)
			{
				found = true;
				break;
			}
		}
		if (found)
			possibleStartPosition.erase(possibleStartPosition.begin() + i);
		return;
	}

	auto Spieler::addAllowedOverwriteTurn(int x, int y, short d) -> void{
		if (!is_allowed(x, y, d)){
			allowedOverwriteTurn.push_back(turnPos{ x, y });
		}
	}

	auto Spieler::addAllowedOverwriteTurnX(int x, int y, short d) -> void{
		if (!is_allowed(x, y, d))
			allowedOverwriteTurnX.push_back(turnPos{ x, y });
	}



	auto Spieler::clearValidTurns() -> void{
		allowedTurn.clear();
		allowedOverwriteTurn.clear();
		allowedOverwriteTurnX.clear();
	}

	auto Spieler::clearValidTurnswoX() -> void{
		allowedTurn.clear();
		allowedOverwriteTurn.clear();
	}


	auto Spieler::clearStartPos() -> void{
		
		possibleStartPosition.clear();
	}


	auto Spieler::getPossibleStartPosition() -> std::vector<turnPos>{
		return possibleStartPosition;
	}

	auto Spieler::getPlayerNumber() -> short{
		return playerNumber;
	}

	auto Spieler::getPlayerChar() -> char{
		return playerChar;
	}

	auto Spieler::getNumOverrideStones() -> unsigned short{
		return numberOfOverwritestone;
	}

	auto Spieler::getAllowedTurns() -> std::vector < turnPos > {
		return allowedTurn;
	}

	auto Spieler::getAllowedOverwriteTurns() -> std::vector < turnPos > {
		return allowedOverwriteTurn;
	}

	auto Spieler::getAllowedOverwriteTurnsX() -> std::vector < turnPos > {
		return allowedOverwriteTurnX;
	}

	auto Spieler::outputData() -> void{
		std::cout << "Spielernr: " << playerNumber << std::endl
			<< "Bombs: " << numberOfBombs << std::endl
			<< "Ueberschreib: " << numberOfOverwritestone << std::endl
			<< "Startpositionen: " << std::endl;
		for (auto start : possibleStartPosition)
			std::cout << "X: " << start.pos_X << " Y: " << start.pos_Y << std::endl;

		for (auto allowed : allowedTurn)
			std::cout << "X: " << allowed.pos_X << " Y: " << allowed.pos_Y << std::endl;

		for (auto allowed : allowedOverwriteTurn)
			std::cout << "X: " << allowed.pos_X << " Y: " << allowed.pos_Y << std::endl;

		std::cout << std::endl;

		return;
	}
}