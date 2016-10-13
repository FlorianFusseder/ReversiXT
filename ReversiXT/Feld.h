#ifndef FELD_H
#define FELD_H
#include <iostream>
#include "Spieler.h"

namespace ff{

	struct transData{
		turnPos turnPos;
		short direction;
	};


	struct Feld{

		enum Art : char
		{
			Hole = '-',
			Empty = '0',
			Choice = 'c',
			Inversion = 'i',
			Bonus = 'b',
			Expansion = 'x'
		};

		enum Wert : int
		{
			X					= 10,	//10
			Normal				= 10,	//10
			Ecke				= 300,	//10000
			NebenEcke			= -100,	//-200
			BonusW				= 150,	//200
			NebenBonus			= -30,	//-90
			ChoiceW				= 350,	//100
			NebenChoice			= -350,	//-500
			Inversition			= 10,	//100
			NebenInversition	= -10,	//-100
			SteinMulti			= 5
		};


		
		char art;														//Identifikationszeichen
		bool playerField;												//spieleridentifikation True wenn Spieler drauf liegt
		short score;
		std::unordered_map<short, transData> Transition;
		Feld() :art(Art::Hole), playerField(false), score(0), Transition(){}
		Feld(char c) :art(c), playerField(false), score(0), Transition(){}
	};

}
#endif