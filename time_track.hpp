/*
 * time_track.hpp
 *
 *  Created on: Jan 24, 2015
 *      Author: anedospe
 */

#ifndef TIME_TRACK_HPP_
#define TIME_TRACK_HPP_

#include <chrono>
#include <iostream>

namespace nxm{

class TimeTrack{
private:

	std::chrono::time_point<std::chrono::system_clock> begin, end;

	TimeTrack(const TimeTrack&);
	TimeTrack& operator=(const TimeTrack&);

public:
	TimeTrack(){}

	void start(){
		begin = std::chrono::high_resolution_clock::now();
	}

	void stop(){
		end = std::chrono::high_resolution_clock::now();
		std::cout << "Dur in nanosec: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << std::endl;
	}

};


}; // end of namespace nxm

#endif /* TIME_TRACK_HPP_ */
