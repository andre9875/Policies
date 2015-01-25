#ifndef _FILE_POLICE_H_
#define _FILE_POLICE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>


namespace nxm{

class FileUncompress{
private:

   FileUncompress& operator=(const FileUncompress&); //disable assigment
   FileUncompress(const FileUncompress&);            //disable copy ctor

protected:

    const int bufsize;
	std::fstream fFile;

	  ~FileUncompress() { } //the destructor is protected, only derived classes can destroy policy object

	  void openRead(const std::string &filename,std::ios_base::openmode m_mode = std::ios_base::in){
         try{
	        fFile.open(filename.c_str(), m_mode); //Create file.
            if(!fFile.is_open()) throw std::string("cannot open file");
	     }catch(...){
			throw std::string("cannot create file");
	     }
	  }

	  void openWrite(const std::string &filename,std::ios_base::openmode m_mode = std::ios_base::ate|std::ios_base::out){
        try{
	      fFile.open(filename.c_str(), m_mode); //Create file.
          if(!fFile.is_open()) throw std::string("cannot open file");
		}catch(...){
			throw std::string("cannot create file");
		}
	  }

	  bool readLine(std::string &buf){
		  try{
			char buffer[bufsize];
		    fFile.getline(buffer, bufsize);
		    buf = std::string(buffer);
		  }catch(...){
			return false;
		  }
		  return fFile.eof();
	  }

	  inline void writeEndl() { fFile << std::endl; }

	  void writeLine(const std::string &buf) {
         try{
            fFile << buf;
		 }catch(...){
			 throw std::string("cannot write to the file");
		 }
	  }

	  inline void closeFile(){ if(fFile.is_open()) fFile.close(); }

public:

	FileUncompress(const int bufsize_ = 1024000) :
	  bufsize(bufsize_)
	  {}


};

}; //end of nxm namespace

#endif
