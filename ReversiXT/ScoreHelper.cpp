#include "ScoreHelper.h"


namespace ms {
	ScoreHelper::ScoreHelper()
	{
	}


	ScoreHelper::~ScoreHelper()
	{
	}
	int ScoreHelper::nextDir(int dir) {
		if (dir == 7) return 0;
		else return dir+1;

	}

	bool ScoreHelper::isWall(std::vector<std::vector<ff::Feld>>& map, int x, int y, int dir, int maxX, int maxY) {

		if (map.at(y).at(x).Transition.find(dir) != map.at(y).at(x).Transition.end()) return false; // Has transition in the direction

		switch (dir)
		{
		case 0:
			if (y == 0) return true;
			return map.at(y - 1).at(x).art == ff::Feld::Art::Hole ? true : false;
			break;
		case 1:
			if (y == 0 || x == maxX) return true;
			return map.at(y - 1).at(x + 1).art == ff::Feld::Art::Hole ? true : false;
			break;
		case 2:
			if (x == maxX) return true;
			return map.at(y).at(x + 1).art == ff::Feld::Art::Hole ? true : false;
			break;
		case 3:
			if (y == maxY || x == maxX) return true;
			return map.at(y + 1).at(x + 1).art == ff::Feld::Art::Hole ? true : false;
			break;
		case 4:
			if (y == maxY) return true;
			return map.at(y + 1).at(x).art == ff::Feld::Art::Hole ? true : false;
			break;
		case 5:
			if (y == maxY || x == 0) return true;
			return map.at(y + 1).at(x - 1).art == ff::Feld::Art::Hole ? true : false;
			break;
		case 6:
			if (x == 0) return true;
			return map.at(y).at(x - 1).art == ff::Feld::Art::Hole ? true : false;
			break;
		case 7:
			if (y == 0 || x == 0) return true;
			return map.at(y - 1).at(x - 1).art == ff::Feld::Art::Hole ? true : false;
			break;
		default: // BUG
			return false;
			break;
		}
	}

	void ScoreHelper::findEdges(std::vector<std::vector<ff::Feld>>& map, unsigned short mul) {

		int xmax = map.at(0).size() - 1;
		int ymax = map.size() - 1;
		
		int x = 0, y = 0;
		for (auto &FeldVec : map) {
			x = 0;
			for (auto &tempFeld : FeldVec) {

				if (tempFeld.art != ff::Feld::Art::Hole) {

					for (int i = 0; i <= 7; i+=2) {

						if (isWall(map, x, y, i, map.at(0).size() - 1, map.size() - 1) && isWall(map, x, y, nextDir(i), map.at(0).size() - 1, map.size() - 1) && isWall(map, x, y, nextDir(nextDir(i)), map.at(0).size() - 1, map.size() - 1)) {
							map.at(y).at(x).score += ff::Feld::Wert::Ecke * mul;					//bewertung feld

							for (int addy = -1; addy <= 1; addy++)															//außenrum (richtungen quasi)...
								for (int addx = -1; addx <= 1; addx++)														//...werte für felder
									if (!(addx == 0 && addy == 0))															// außer 0, 0 -> is ja genau das feld selbst
										if (x + addx >= 0 && y + addy >= 0 && x + addx <= xmax && y + addy <= ymax && map[y + addy][x + addx].art != ff::Feld::Art::Hole)			//wenn nich out of bounds und kein leeres feld
											map[y + addy][x + addx].score += ff::Feld::Wert::NebenEcke * mul; //bewertung vor feld
						}
					}
				}
				x++;
			}
			y++;
		}

	}
}
