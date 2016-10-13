#include "Evaluator.h"

namespace ms {

	Evaluator::Evaluator()
	{
	}


	Evaluator::~Evaluator()
	{
	}

	int Evaluator::getValue(short player, ff::Spiel& game)
	{
	
		std::vector<int>scoreBoard;
		ms::ScoreCollector::getScore(scoreBoard, game, player);
		return scoreBoard[0];
		
	}



	void Evaluator::getValueAll(ff::Spiel& game, std::vector<int>& scoreBoard)
	{

		for (unsigned int i = 0; i < game.getPlayers().size() && game.activePlayers[i]; i++){

			scoreBoard.push_back(ms::Evaluator::getValue(i + 1, game));
		}

	}

	int Evaluator::getValueSimple(short player, ff::Spiel& game)
	{

		return ms::ScoreCollector::getSimpleScore(game, player);

	}


	int Evaluator::getDiff(short player, ff::Spiel& game)
	{
		int tempSum = 0;
		std::vector<int>scoreBoard;
		ms::Evaluator::getValueAll(game, scoreBoard);
		for (unsigned int i = 0; i < game.getPlayers().size() && game.activePlayers[i]; i++){
			if (i + 1 == player) tempSum += scoreBoard[i];
			else tempSum -= scoreBoard[i ];
		}
		return tempSum;
	}

	std::vector<rank> Evaluator::getRanking(ff::Spiel& game){

		std::vector<rank> s;
		for (int i = 1; i < game.getSpielerzahl() + 1;i++){
			s.push_back(rank{i, ms::Evaluator::getValueSimple(i, game) });
		}

		return s;
	}
}