#ifndef nxm_VideoLIB_H
#define nxm_VideoLIB_H

#include "nxm_filelib.h"
#include "nxm_utllib.h"
//#include "nxm_dblib.h"
#include "nxm_split.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <exception>

/*
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/bind.hpp>
#include <boost/move/move.hpp>
*/

namespace nxm_video{

//Typedefs of allocators and containers
/*
typedef boost::interprocess::managed_shared_memory::segment_manager segment_manager_t;
typedef boost::interprocess::allocator<void, segment_manager_t> void_allocator;
typedef boost::interprocess::allocator<char, segment_manager_t> char_allocator;
typedef boost::interprocess::basic_string<char, std::char_traits<char>, char_allocator> char_string;
typedef boost::interprocess::allocator<char_string, segment_manager_t> string_allocator;
typedef boost::interprocess::vector<char_string, string_allocator> vector_type;
*/

typedef std::map<std::string, long> _MapType;
typedef std::pair<std::string,long> _MapPair;
typedef std::map<std::string, int> _MapTypeInt;
typedef std::pair<std::string,int> _MapPairInt;
typedef std::vector<int> _VectorTypeInt;
typedef std::vector<std::string> _VectorTypeStr;

 class nxm_Video : nxm_split::nxmSplit
 {
  public:

   const char* _delim;

   nxm_Video(const std::string&, const std::string&, const int);

   void count_videos();
   void set_program_id(const int, const std::string&);

   ~nxm_Video() {}

  private:

   //Disable copy constructor and assignment operator
   nxm_Video(const nxm_Video&); // {}
   nxm_Video& operator=(const nxm_Video&); //{}


   _VectorTypeStr summary_key;
   std::string database;
   std::string _email;
   std::string open_file;
   int day;
   int sunday;
   int pid;
   static const int _gap=1800;
   static const int last_dur=2;
   char _type;
   std::string script_name;
   std::string file_name;
   std::string shm_name;
   int _dma_flag;

   void init_key(const _VectorTypeStr&,const _VectorTypeStr&, _VectorTypeInt&);
   void init_key_video(const _VectorTypeStr&,const _VectorTypeStr&, _VectorTypeInt&);
   void init_key_uu(const _VectorTypeStr&,const _VectorTypeStr&, _VectorTypeInt&);

   void create_sqlloader_file(const _MapType&,const _MapType&,const _MapType&,const _MapType&);
   void read_sqlloader_keys(_VectorTypeStr&);
   void write_err(std::string);

 };

};
#endif


