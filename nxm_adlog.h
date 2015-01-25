#ifndef nxm_ADLOGLIB_H
#define nxm_ADLOGLIB_H

#include "nxm_adlog.h"
#include "nxm_filelib.h"
#include "nxm_utllib.h"
//#include "nxm_dblib.h"
#include <string>
#include <iostream>
#include <exception>

#include <ctype.h>

namespace nxm_adlog{

typedef std::map<std::string, int> _MapType;
typedef std::vector<int> _VectorTypeInt;
typedef std::vector<std::string> _VectorTypeStr;

 class nxm_Adlog
 {
  public:

   const char* _delim;

   nxm_Adlog(std::string&,std::string*, int&);
   void count_user_visits();
   void set_key(_VectorTypeStr*,_VectorTypeStr*,std::string&);
   void set_program_id(int, std::string);
   void set_summary_key(_VectorTypeStr *, std::string&,int);
   void create_sqlloader_file(_MapType*,_MapType*,_MapType*,_MapType*);
   void read_sqlloader_keys(_VectorTypeStr *);
   void write_err(std::string);

   void split(_VectorTypeStr* , const char* , const char * );
   void split_newline(_VectorTypeStr* , const char* , const char * );

   void replaceChar(std::string&, char);


  private:

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
   //std::string shm_name;
   int _dma_flag;


 };

};
#endif


