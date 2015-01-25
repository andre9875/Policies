#ifndef UIM_FILE_H
#define UIM_FILE_H

#include <stdio.h>
#include <cstdio>
#include <stdexcept>  // std::runtime_error


namespace nxm_file{

 class nxm_File
  {
   public:
    nxm_File (const char* , const char* );
    nxm_File (const char*);
   ~nxm_File ();
    void write (const char* );
    //void close ();
   private:
    std::FILE* pFile;
  };

};
#endif

