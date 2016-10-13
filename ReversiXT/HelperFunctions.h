#ifndef HELPERFUNCTION_H
#define HELPERFUNCTION_H
#include "Spiel.h"
#include "Spieler.h"
#include <bitset>
class HelperFunctions
{
public:
	HelperFunctions();
	~HelperFunctions();

	/// <summary>
	/// Checks if a player is still active
	/// </summary>
	/// <param name = "playerNumber">Playernumber< / param>
	/// <param name = "game">Spielobject< / param>
	/// <returns>Activity of the player</returns>
	static bool playerIsActive(int playerNumber, ff::Spiel& game);

	/// <summary>
	/// Disqualifies a player
	/// </summary>
	/// <param name = "playerNumber">Playernumber< / param>
	/// <param name = "game">Spielobject< / param>
	static void disqualifyPlayer(int playerNumber, ff::Spiel &game);

};
#endif
