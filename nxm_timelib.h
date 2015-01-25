#ifndef UIM_TIMELIB_H
#define UIM_TIMELIB_H

#include <time.h>
#include <string>

namespace nxm_time{


 class nxm_Time
 {
  public:
   std::string get_current_date();
   std::string get_current_time();
   std::string get_nxm_date();
   int get_nxm_date_int();
   int add_day(int, int);		//YYYYMMDD DD
   int subtract_day(int, int);	//YYYYMMDD DD
   bool is_leap_year(int);	 	//YYYY
   int last_month_day(int);	 	//YYYYMM
   int get_monday(int);			//YYYYMMDD
   int return_hour(time_t);		//UnixTime
   std::string return_date_time(time_t);	//UnixTime
   //long long conv_date_unixtime(long long) ;	//UnixTime
   int conv_date_unixtime(const std::string *) ;	//UnixTime
   int day_of_week(int &, int &, int &); // DD MM YYYY
   int day_of_week(std::string &); // YYYYMMDD
   int day_of_week(int &); // YYYYMMDD
 };

};
#endif
