#include "clock.h"

namespace repu{
	clock::clock()
	{
		start = end = std::chrono::high_resolution_clock::now();
	}


	clock::~clock()
	{
	}


	void clock::restart(){
		start = std::chrono::high_resolution_clock::now();
	}

	long long clock::restartAsMilSecs(){
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
		start = std::chrono::high_resolution_clock::now();
		return millis.count();
	}

	double clock::restartAsSecs(){
		auto d_secs = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - start);
		start = std::chrono::high_resolution_clock::now();
		return d_secs.count();
	}

	long long clock::getAsMilSecs(){
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
		return millis.count();
	}

	double clock::getAsSecs(){
		auto d_secs = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - start);
		return d_secs.count();
	}

}