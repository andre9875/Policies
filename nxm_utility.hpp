#ifndef _NXM_UTIL_H_
#define _NXM_UTIL_H_

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


}; // end of namespace nxm

#endif

