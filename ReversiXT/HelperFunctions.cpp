#include "HelperFunctions.h"



HelperFunctions::HelperFunctions()
{
}


HelperFunctions::~HelperFunctions()
{
}

 bool HelperFunctions::playerIsActive(int playerNumber, ff::Spiel& game) {

	return game.activePlayers.test(playerNumber-1);
}


 void HelperFunctions::disqualifyPlayer(int playerNumber, ff::Spiel &game) {
	game.activePlayers.set(playerNumber, 0);
}