/*
 * IPLocation.hpp
 *
 *  Created on: Jan 31, 2015
 *      Author: anedospe
 */

#ifndef IPLOCATION_HPP_
#define IPLOCATION_HPP_


#include "IP2Location.h"
#include <iostream>

namespace nxm{

class IPLocation{
	private:
		char *file;
		IP2Location *ptrIPLoc;
		IP2LocationRecord *record;


	public:
		IPLocation(char *_file) : file(_file)
		{
			ptrIPLoc = IP2Location_open(file);
		}

		~IPLocation(){
			IP2Location_close(ptrIPLoc);
		}

		void convertIP(char *ip){
			record = IP2Location_get_all(ptrIPLoc, ip);

			std::cout << record->country_short << std::endl;
			std::cout << record->country_long << std::endl;
			std::cout << record->region << std::endl;
			std::cout << record->city << std::endl;
			std::cout << record->latitude << std::endl;
			std::cout << record->longitude << std::endl;

			IP2Location_free_record(record);
		}

}; // end of class


}; //end of namespace nxm



#endif /* IPLOCATION_HPP_ */
