#ifndef ALGORITHMBASE_H
#define ALGORITHMBASE_H

#include "Spiel.h"
#include "Spieler.h"
#include "ExecTurn.h"
#include "Evaluator.h"
#include "Turns.h"
#include "HelperFunctions.h"
#include "clock.h"
#include <vector>
#include <iostream>
#include <thread>
#include <algorithm>

#ifdef __unix__
#define INT_MAX std::numeric_limits<int>::max()
#define INT_MIN std::numeric_limits<int>::min()
#endif



namespace ms {

	struct abp{
			int alpha = std::numeric_limits<int>::min();
			int value = std::numeric_limits<int>::min();
			int beta = std::numeric_limits<int>::max();
	};

	struct sortStruct{
		int value;
		int gID;
	};


	class AlgorithmBase
	{
	private:

		bool debug;
		bool noSort;

		bool cutFlag = false;			//vs (vs2k13 ?) has some issues with bools initialized in headers... 			//important to cut not needed turns
		int zaehler = 0;																								//only for counting from total turns needed
		int totalMax = 0;			
		ff::turnPos Turn_t;
		bool run;
		int threadDepth = 0;
		int aspDelta = 0;
		int tValue = 0;

		/// <summary>
		/// Builds the searchtree
		/// </summary>
		/// <param name = "player">Playernumber< / param>
		/// <param name = "maxDepth">maximum depth< / param>
		/// <param name = "depth">actual depth of the searchtree< / param>
		/// <param name = "wertung">alpah-beta-pruning struct< / param>
		/// <returns>Value of the searchtree</returns>
		int MiniMax(ff::Spiel& game, short player, int maxDepth, int depth, abp wertung);
		
		int MiniMaxUnsorted(ff::Spiel& game, short player, int maxDepth, int depth, abp wertung);	//UNUSED
		int MiniMaxTotal(ff::Spiel& game, short player, int maxDepth, int depth, abp wertung);		//UNUSED


		/// <summary>
		/// Sorts the games 
		/// </summary>
		/// <param name = "games">Spielobjects< / param>
		/// <param name = "turns">Turns< / param>
		/// <param name = "player">Playernumber< / param>
		/// <param name = "isMax">Is the sorting for maximizing or minimizing< / param>
		void SortGame(std::vector<ff::Spiel>& games, std::vector<ff::turnPos>& turns, short player,bool isMax);
		
		/// <summary>
		/// Compares the sortStructs for the Max
		/// </summary>
		/// <param name = "a">sortStruct1< / param>
		/// <param name = "b">sortStruct2< / param>
		/// <returns>Comparison</returns>
		static bool CompareStructs(const sortStruct& a,const sortStruct& b);
		

		static bool CompareScore(const rank& a, const rank& b);

		/// <summary>
		/// Compares the sortStructs for the Max
		/// </summary>
		/// <param name = "a">sortStruct1< / param>
		/// <param name = "b">sortStruct2< / param>
		/// <returns>Comparison</returns>
		static bool CompareStructsMin(const sortStruct& a, const sortStruct& b);
																

		/// <summary>
		/// Function for the calculation of the searchtree
		/// </summary>
		/// <param name = "game">Spielobject< / param>
		/// <param name = "player">Playernumber< / param>
		void MMSearchThread(ff::Spiel game, short player);


		int maxValue;
		abp wWertung;
		int aWmin = INT_MIN;
		int aWmax = INT_MAX;

	public:
		AlgorithmBase();

		AlgorithmBase(bool _debug, bool _noSort);
		~AlgorithmBase();
		void setDebug(bool _debug);
		void setNoSort(bool _noSort);

		/// <summary>
		/// Calculates the turn that should be taken, given a depth
		/// </summary>
		/// <param name = "game">Spielobject< / param>
		/// <param name = "player">Playernumber< / param>
		/// <param name = "maxDepth">max depth for the calculation< / param>
		/// <returns>Turn that should be taken</returns>
		ff::turnPos MiniMaxPos(ff::Spiel& game, short player, int maxDepth);
		
		/// <summary>
		/// Calculates the turn that should be taken, given a time
		/// </summary>
		/// <param name = "game">Spielobject< / param>
		/// <param name = "player">Playernumber< / param>
		/// <param name = "time">max time for the calculation< / param>
		/// <returns>Turn that should be taken</returns>
		ff::turnPos MiniMaxDeep(ff::Spiel& game, short player, uint32_t time);
		
		/// <summary>
		/// Calculates the bomb-turn that should be taken
		/// </summary>
		/// <param name = "">Spielobject< / param>
		/// <param name = "myPlayerNumber">Playernumber< / param>
		/// <returns>Bomb-turn that should be taken</returns>
		static ff::turnPos bombPos(ff::Spiel&, int myPlayerNumber);
	};
}

#endif