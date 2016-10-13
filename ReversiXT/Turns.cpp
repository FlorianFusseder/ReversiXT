#include "Turns.h"

namespace ff{

	auto reverseDir(short dir) -> short{
		short rDir = dir + 4;
		if (rDir > 7) rDir = rDir - 8;
		return rDir;
	}

	auto nextStep(ff::Spiel& G, Feld& temp, int x, int y, int Feldabstand, char Spieler, int direction, turnPos start) -> int{

		if (Feldabstand == 1 || Feldabstand == -1000){													//Ist das Feld ein Nachbarfeld kann kann es kein erlaubter zug sein
			if (!temp.playerField || temp.art == Spieler)												//Ist das Feld dazu noch ein Feld auf dem Kein Spieler oder der Spieler selbst steht -> hier is kein zug möglich
				return 0;																				//0 -> break;
			if (!temp.Transition.empty()){																//Besitz das Feld Transitionen
					return 2;																		
			}
			return 1;																					//1 -> continue;
		}

		if (temp.playerField){																			//Ist das Feld von einem Spieler besetzt kann es auf jeden fall mit einem Überschreibstein überschrieben werden
			if (G.getPlayer(Spieler).getNumOverrideStones() > 0)
				if (!(start.pos_X == x && start.pos_Y == y))
					G.addAllowedOverwrite(Spieler, x, y, -1);												//-> also zu Vector hinzufügen
			if (temp.art == Spieler && Feldabstand != 0)												//Wenn der Spieler auf sich selbst trifft kann abgebrochen werden -> kein erlaubter Zug mehr möglich
				return 0;																				//0 -> break;
		}
		else{																							//Gibt es keinen Spieler dem das Feld gehört trifft else zu
			if (temp.art != Feld::Art::Hole){															//Ist das Feld nicht von der Art '-'
				G.addAllowedTurn(Spieler, x, y, reverseDir(direction));												//Kann es nur noch ein erlaubter zug sein
				return 0;																				//0 -> break;
			}
			else{
				return 2;																				//2 -> Prüfen auf Transition
			}
		}
		return 2;
	}

	auto ifTrans(ff::Spiel& G, Spieler& player, Feld temp, transData neu, int Stepcounter, turnPos start) -> void{

		Feld akt = G.getFeld(neu.turnPos.pos_X, neu.turnPos.pos_Y);																								//Speicherpunkt des aktuellen Feldes
		Feld before = temp;
		short direction = reverseDir(neu.direction);
		//Speicherpunkt des Feldes das zuvor besucht wurde
		Stepcounter++;

		if ((player.getPlayerChar() == before.art && before.art == akt.art))				//Ist das das Feld nach der Transaktion vom selben besetzt wie von dem Feld zuvor(before)
			return;																		//...wird abgebrochen da es für den untersuchten stein keinen erlaubten Zug geben kann

		int zustand = nextStep(G, akt, neu.turnPos.pos_X, neu.turnPos.pos_Y, Stepcounter, player.getPlayerChar(), direction, start);

		if (zustand == 0)
			return;
		if (zustand == 2)
		{
			if (akt.Transition.find(direction) != akt.Transition.end())																		//in die richtige richtung?!
				ifTrans(G, player, akt, akt.Transition.at(direction), Stepcounter, start);	
		}

		if (Stepcounter == -1000)																														// für den fall das gleich auf dem startfeld eine transition vorhanden wird ifTrans mit -2 (flag) aufgerufen oben wird dann gleich eins draufgezählt darum muss der zähler für den Aufruf von...
			Stepcounter = 2;																															//...checkLegitturn auf 2 gesetzt werden, da checklegitturn normalerweiße sofort mit 1 initialierst wird gleich ein feld weiter startet		
																																						// und an einem feld weiter überprüft sonst stimmt der Feldabstand nicht
		switch (direction){
		case 4:
			checkLegitTurn(G, player, akt, neu.turnPos, Stepcounter, 0, 1, 4, start);													//überprüfung oben
			break;
		case 3:
			checkLegitTurn(G, player, akt, neu.turnPos, Stepcounter, 1, 1, 3, start);													//überprüfung oben rechts
			break;
		case 2:
			checkLegitTurn(G, player, akt, neu.turnPos, Stepcounter, 1, 0, 2, start);													//überprüfung rechts
			break;
		case 1:
			checkLegitTurn(G, player, akt, neu.turnPos, Stepcounter, 1, -1, 1, start);												//überprüfung unten rechts
			break;
		case 0:
			checkLegitTurn(G, player, akt, neu.turnPos, Stepcounter, 0, -1, 0, start);												//überprüfung unten
			break;
		case 7:
			checkLegitTurn(G, player, akt, neu.turnPos, Stepcounter, -1, -1, 7, start);												//überprüfung unten links
			break;
		case 6:
			checkLegitTurn(G, player, akt, neu.turnPos, Stepcounter, -1, 0, 6, start);												//überprüfung links
			break;
		case 5:
			checkLegitTurn(G, player, akt, neu.turnPos, Stepcounter, -1, 1, 5, start);												//überprüfung links oben
			break;
		}
	}

	auto checkLegitTurn(ff::Spiel& G, Spieler& player, Feld& temp, turnPos start, int schrittzahl, int addX, int addY, int direction, turnPos startalt) -> void{

		int zustand;

		for (int x = start.pos_X + addX, y = start.pos_Y + addY, Feldabstand = schrittzahl; temp.art != '-'; x += addX, y += addY, Feldabstand++){	//solange das Feld von einem Spieler besetzt ist gehe weiter
			try{																															//try ein Feld zu holen bricht ab falls out of bounds
				temp = G.getFeld(x, y);																										//Hier wird das zu überpüfende feld zwischengespeichert
			}
			catch (const std::out_of_range oor){		
				break;
			}

			zustand = nextStep(G, temp, x, y, Feldabstand, player.getPlayerChar(), direction, startalt);										//Prüft auf erlaubten Zug und speichert weiteren verlauf in Zustand
			
			if (zustand == 0){																											//nähere Beschreibung in der Funktion addTurn nachzulesen											
				break;
			}
			else if (zustand == 1){
				continue;
			}
			else{																															//Muss aufgrund des Rückgabewertes '-' Feld sein
				auto got = temp.Transition.find(direction);																		//Überprüfen ob es Transition auf dem Feld gibt, falls es mehrere gibt...
				if (got != temp.Transition.end() && got->first == direction){													//... wird auf die richtige richtung überprüft
					ifTrans(G, player, temp, temp.Transition.at(direction), Feldabstand, startalt);											//falls ja wird ab Transitionsende weiter geprüft
					break;																													//nach ifTrans wurde ein Ende gefunden und der Derzeitige ablauf kann abgebrochen werden
				} else{
					if (temp.art == Feld::Art::Hole){
						break;
					}
				}
			}
		}
	}

	auto startTurns(ff::Spiel& G) -> void{

		for (auto &player : G.getPlayers()){																		//für jeden Spieler
			for (auto &start : player.getPossibleStartPosition()){															//für jede Startpostition
				Feld temp = G.getFeld(start.pos_X, start.pos_Y);													//Feld der Startposition 
				Feld akt = temp;																					//Zwischenspeichern
				turnPos startalt = start;
				

				if (!akt.Transition.empty()){																					//Überprüfen ob es Transition auf dem ersten Feld gibt, falls es mehrere gibt...
					for (auto it = akt.Transition.begin(); it != akt.Transition.end(); it++){									//... wird auf die erste richtung überprüft
						ifTrans(G, player, akt, temp.Transition.at(it->first), -1001, start);
					}
				}

				for (int dir = 0; dir < 8; dir++){
		
					switch (dir){
					case 4:
						checkLegitTurn(G, player, akt, start, 1, 0, 1, 4, startalt);													//überprüfung oben
						akt = temp;
						break;
					case 3:
						checkLegitTurn(G, player, akt, start, 1, 1, 1, 3, startalt);													//überprüfung oben rechts
						akt = temp;
						break;
					case 2:
						checkLegitTurn(G, player, akt, start, 1, 1, 0, 2, startalt);													//überprüfung rechts
						akt = temp;
						break;
					case 1:
						checkLegitTurn(G, player, akt, start, 1, 1, -1, 1, startalt);													//überprüfung unten rechts
						akt = temp;
						break;
					case 0:
						checkLegitTurn(G, player, akt, start, 1, 0, -1, 0, startalt);													//überprüfung unten
						akt = temp;
						break;
					case 7:
						checkLegitTurn(G, player, akt, start, 1, -1, -1, 7, startalt);												//überprüfung unten links
						akt = temp;
						break;
					case 6:
						checkLegitTurn(G, player, akt, start, 1, -1, 0, 6, startalt);													//überprüfung links
						akt = temp;
						break;
					case 5:
						checkLegitTurn(G, player, akt, start, 1, -1, 1, 5, startalt);													//überprüfung links oben
						akt = temp;
						break;
					}
				}
			}
		}
	}

	auto startTurns(ff::Spiel& G, short PlayerNumber) -> void{

		ff::Spieler & player = G.getPlayer(PlayerNumber);														//für jeden Spieler
		for (auto &start : player.getPossibleStartPosition()){															//für jede Startpostition
			Feld temp = G.getFeld(start.pos_X, start.pos_Y);													//Feld der Startposition 
			Feld akt = temp;																					//Zwischenspeichern
			turnPos startalt = start;
			

			if (!akt.Transition.empty()){																					//Überprüfen ob es Transition auf dem ersten Feld gibt, falls es mehrere gibt...
				for (auto it = akt.Transition.begin(); it != akt.Transition.end(); it++){									//... wird auf die erste richtung überprüft
					ifTrans(G, player, akt, temp.Transition.at(it->first), -1001, start);
				}
			}
			
			for (int dir = 0; dir < 8;dir++){
				
				switch (dir){
				case 4:
					checkLegitTurn(G, player, akt, start, 1, 0, 1, 4, startalt);													//überprüfung oben
					akt = temp;
					break;
				case 3:
					checkLegitTurn(G, player, akt, start, 1, 1, 1, 3, startalt);													//überprüfung oben rechts
					akt = temp;
					break;
				case 2:
					checkLegitTurn(G, player, akt, start, 1, 1, 0, 2, startalt);													//überprüfung rechts
					akt = temp;
					break;
				case 1:
					checkLegitTurn(G, player, akt, start, 1, 1, -1, 1, startalt);													//überprüfung unten rechts
					akt = temp;
					break;
				case 0:
					checkLegitTurn(G, player, akt, start, 1, 0, -1, 0, startalt);													//überprüfung unten
					akt = temp;
					break;
				case 7:
					checkLegitTurn(G, player, akt, start, 1, -1, -1, 7, startalt);												//überprüfung unten links
					akt = temp;
					break;
				case 6:
					checkLegitTurn(G, player, akt, start, 1, -1, 0, 6, startalt);													//überprüfung links
					akt = temp;
					break;
				case 5:
					checkLegitTurn(G, player, akt, start, 1, -1, 1, 5, startalt);													//überprüfung links oben
					akt = temp;
					break;
				}
			}
		}
	}
	
}