
#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "Spiel.h"
#include "ScoreCollector.h"
#include <vector>
#include <map>
#include <algorithm>


namespace ms {

		struct rank{
		int player;
		int score;
	};



	class Evaluator
	{

	public:
		Evaluator();
		~Evaluator();

		/// <summary>
		/// Calculate the score of a player
		/// </summary>
		/// <param name = "player">Playernumber< / param>
		/// <param name = "game">Spielobject< / param>
		/// <returns>Score for a player</returns>
		static int	getValue(short player, ff::Spiel& game); 
		
		/// <summary>
		/// Calculate the score for all players
		/// </summary>
		/// <param name = "game">Spielobject< / param>
		/// <param name = "scoreBoard">Scoreboard< / param>	
		static void	getValueAll( ff::Spiel& game, std::vector<int>& scoreBoard); //returns the amount of points for a given player

		/// <summary>
		/// Counts the stones for a player
		/// </summary>
		/// <param name = "player">Playernumber< / param>
		/// <param name = "game">Spielobject< / param>
		/// <returns>Stones of a player</returns>
		static int	getValueSimple(short player, ff::Spiel& game); 

		/// <summary>
		/// Calculates the difference of the player to the others
		/// </summary>
		/// <param name = "player">Playernumber< / param>
		/// <param name = "game">Spielobject< / param>
		/// <returns>Score for a player</returns>
		static int	getDiff(short player, ff::Spiel& game); 
		
		/// Gibt momentane Plätze aus
		static std::vector<rank> getRanking(ff::Spiel& game);
	};
}
#endif
