#ifndef EXECTURN_H
#define EXECTURN_H

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include "Feld.h"
#include "Spiel.h"
#include "Spieler.h"

#include "Turns.h"


namespace repu{
	////////////////////////////////////////////////////////////////////////////////
	/// \brief Executes a turn
	///
	/// \param	Game		Instance of Game
	/// \param	Player		Instance of executing Player
	/// \param	coords		Coordinates (including direrction) of turn to execute
	/// \param	choice		Optional! Char of Player color gets changed with
	/// \param	bonus		Optional! Choice of extra Bomb or OverwriteStone 
	///						1 = Bomb; 2 = OverrideStone
	///
	////////////////////////////////////////////////////////////////////////////////
	void execTurn(ff::Spiel& Game, ff::Spieler& Player, ff::turnPos coords);
	void execTurn(ff::Spiel& Game, ff::Spieler& Player, ff::turnPos coords, char choice);
	void execTurn(ff::Spiel& Game, ff::Spieler& Player, ff::turnPos coords, short bonus);
	void execTurn(ff::Spiel& Game, ff::Spieler& Player, ff::turnPos coords, char choice, short bonus);


	struct bombStep{
		int step;
		ff::turnPos coords;
	};

	void execBomb(ff::Spiel& Game, repu::bombStep coords);
}

#endif //EXECTURN_H