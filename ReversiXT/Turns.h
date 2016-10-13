#ifndef TURNS_H
#define TURNS_H

#include <iostream>
#include <vector>
#include <stdexcept>

#include "Feld.h"
#include "Spiel.h"
#include "Spieler.h"

namespace ff{

	auto reverseDir(short) -> short;
	auto ifTrans(ff::Spiel&, Feld, transData, int, turnPos) -> void;					//Falls Transition
	auto checkLegitTurn(ff::Spiel&, Spieler&, Feld&, turnPos, int, int, int, int, turnPos) -> void;		//�berpr�fung auf Legtimen Weg
	auto nextStep(ff::Spiel&, Feld&, int, int, int, char, int, turnPos) -> int;							//Wie soll weiterverfahren werden
	auto startTurns(ff::Spiel&) -> void;														//Hubfunktion
	auto startTurns(ff::Spiel&, short) -> void;													//�berladene Funktion f�r nur einen Spieler
}

#endif