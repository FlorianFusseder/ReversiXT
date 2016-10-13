#ifndef SCOREHELPER_H
#define SCOREHELPER_H

#include <vector>
#include <iostream>
#include "Feld.h"
namespace ms {
	class ScoreHelper // Contains different functions needed to calculate the score
	{
	private:

		/// <summary>
		///  Checks if a certain position is a wall
		/// </summary>
		/// <param name = "map">Map object< / param>
		/// <param name = "x">X-Position of the position< / param>
		/// <param name = "y">Y-Position of the position< / param>
		/// <param name = "dir">Direction to check< / param>
		/// <param name = "maxX">Maximum X value of the map< / param>
		/// <param name = "maxY">Maximum Y value of the map< / param>
		/// <returns>Is the position a wall</returns>
		static bool isWall(std::vector<std::vector<ff::Feld>>& map,int x, int y,int dir,int maxX,int maxY);

		/// <summary>
		///  Calculates the next direction
		/// </summary>
		/// <param name = "dir">Startdirection< / param>
		/// <returns>Next Direction</returns>
		static int nextDir(int dir);
	public:

		ScoreHelper();
		~ScoreHelper();

		/// <summary>
		///  Finds the Edges on a Map
		/// </summary>
		/// <param name = "map">Map object< / param>
		/// <param name = "mul">Startdirection< / param>
		static void findEdges(std::vector<std::vector<ff::Feld>>& map, unsigned short mul);
	};
}
#endif
