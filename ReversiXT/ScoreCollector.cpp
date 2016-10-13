#include "ScoreCollector.h"

namespace ms {

	ScoreCollector::ScoreCollector()
	{
	}


	ScoreCollector::~ScoreCollector()
	{
	}

	int ScoreCollector::gatherBasicScore(short player, ff::Spiel& _game){
		int result = 0;

		std::vector<std::vector<ff::Feld>>& map = _game.getMap();

		result += _game.getPlayer(player).getAllowedTurns().size() * ff::Feld::Wert::SteinMulti;
		for (auto& posMov : _game.getPlayer(player).getPossibleStartPosition())
		{
			result += map.at(posMov.pos_Y).at(posMov.pos_X).score;
		}

		return result;
	}

	void ScoreCollector::getScore(std::vector<int>& scoreboard, ff::Spiel& game) {
		for (unsigned short s = 1; s <= game.getSpielerzahl(); s++) {
			scoreboard.push_back(gatherBasicScore(s, game));

		}
	}
	void ScoreCollector::getScore(std::vector<int>& scoreboard, ff::Spiel& game, short player) {
		scoreboard.push_back(gatherBasicScore(player, game));
	}

	int ScoreCollector::getSimpleScore(ff::Spiel& game, short player) {
		return game.getPlayer(player).getPossibleStartPosition().size();
	}

}