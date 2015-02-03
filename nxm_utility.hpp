#ifndef _NXM_UTILITY_H_
#define _NXM_UTILITY_H_

#include <string>
#include <sstream>
#include <ctype.h>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>


namespace nxm{
	
	std::string longToString(long _long_value){
		   std::string _string;
		   std::stringstream _stream_str;
		   _stream_str << _long_value;
		   _string = _stream_str.str();
		   return _string;
	}

	void replaceChar(std::string &val, char c){
			for (unsigned int i = 0; i < val.size(); ++i){
					if (val.at(i) == c)
							val.at(i) = ' ';

					if (iscntrl(val.at(i))) //remove control characters
							val.at(i) = ' ';

			}
	}

	size_t date_to_unixtime(const std::string &timestamp){
		//format of timestamp yyyymmddhhmiss +hhmi
		struct tm tm;
		if ( strptime(timestamp.c_str(), "%Y%m%d%H%M%S %Z", &tm) != NULL ){
		  return mktime(&tm);
		}else{
		  return 0;
		}
	}

	std::string int_to_hex( size_t  i ){
	  std::stringstream stream;
	  stream << std::hex << i;
	  return stream.str();
	}

	std::string returnOID(size_t ut){
		return std::string( int_to_hex(ut) + "0000000000000000");
	}
	
	size_t gmt_time(const std::string &date){
		
		std::string y(date,0,4);
		std::string m(date,4,2);
		std::string d(date,6,2);

		int year = atoi(y.c_str());
		int month = atoi(m.c_str());
		int day = atoi(d.c_str());

		time_t rawtime;
		struct tm *timeinfo;

		time ( &rawtime );
		timeinfo = gmtime( &rawtime );

		timeinfo->tm_year = year - 1900;
		timeinfo->tm_mon = month - 1;
		timeinfo->tm_mday = day;
		timeinfo->tm_sec = 0;
		timeinfo->tm_min = 0;
		timeinfo->tm_hour = 0;

		size_t result = mktime ( timeinfo );

		return result;   
	}

	size_t local_time(const std::string &date){
		
		std::string y(date,0,4);
		std::string m(date,4,2);
		std::string d(date,6,2);

		int year = atoi(y.c_str());
		int month = atoi(m.c_str());
		int day = atoi(d.c_str());

		time_t rawtime;
		struct tm *timeinfo;

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );

		timeinfo->tm_year = year - 1900;
		timeinfo->tm_mon = month - 1;
		timeinfo->tm_mday = day;
		timeinfo->tm_sec = 0;
		timeinfo->tm_min = 0;
		timeinfo->tm_hour = 0;

		size_t result = mktime ( timeinfo );

		return result;   
	}	

	size_t time_now(){
		return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}
	

}; // end of namespace nxm

#endif

