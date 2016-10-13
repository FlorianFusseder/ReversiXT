#ifndef SCORECOLLECTOR_H
#define SCORECOLLECTOR_H

#include <vector>
#include "Spiel.h"
#include "Feld.h"
#include "Spieler.h"
#include "Turns.h"


namespace ms {
	class ScoreCollector
	{
	private:
		
		/// <summary>
		/// Gather the basic score
		/// </summary>
		/// <param name = "player">Playernumber< / param>
		/// <param name = "_game">Spielobject< / param>
		/// <returns>Calculates the Score of a map for a player</returns>
		static int gatherBasicScore(short player, ff::Spiel& _game); // Calculates the score for a certain player


	public:
		ScoreCollector();
		~ScoreCollector();
		/// <summary>
		/// Gather the simple score
		/// </summary>
		/// <param name = "game">Spielobject< / param>
		/// <param name = "player">Playernumber< / param>
		/// <returns>Value of the stones on the map</returns>
		static int getSimpleScore(ff::Spiel& game, short player);


		/// <summary>
		/// Gather the score for alle players
		/// </summary>
		/// <param name = "scoreboard">Scoreboard< / param>
		/// <param name = "game">Spielobject< / param>
		/// <returns>Score of the map for all players</returns>
		static void getScore(std::vector<int>& scoreboard, ff::Spiel& game);

		/// <summary>
		/// Gather the score for a player
		/// </summary>
		/// <param name = "scoreboard">Scoreboard< / param>
		/// <param name = "game">Spielobject< / param>
		/// <param name = "player">Playernumber< / param>
		static void getScore(std::vector<int>& scoreboard, ff::Spiel& game,short player); 
		
	};
}

#endif
