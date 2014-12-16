#ifndef _FILE_POLICE_H_
#define _FILE_POLICE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>


namespace uim{

class FileUncompress{
private:

   FileUncompress& operator=(const FileUncompress&); //disable assigment
   FileUncompress(const FileUncompress&);            //disable copy ctor

protected:

    const int bufsize;
	std::fstream fRead;
	std::fstream fWrite;

	  ~FileUncompress() { } //the destructor is protected, only derived classes can destroy policy object

	  void openRead(const std::string &filename,std::ios_base::openmode m_mode = std::ios_base::in){
         try{
	        fRead.open(filename.c_str(), m_mode); //Create file.
            if(!fRead.is_open()) throw std::string("cannot open file");
	     }catch(...){
			throw std::string("cannot create file");
	     }
	  }

	  void openWrite(const std::string &filename,std::ios_base::openmode m_mode = std::ios_base::ate|std::ios_base::out){
        try{
	      fWrite.open(filename.c_str(), m_mode); //Create file.
          if(!fWrite.is_open()) throw std::string("cannot open file");
		}catch(...){
			throw std::string("cannot create file");
		}
	  }

	  bool readLine(std::string &buf){
		  try{
			char buffer[bufsize];
		    fRead.getline(buffer, bufsize);
		    buf = std::string(buffer);
		  }catch(...){
			return false;
		  }
		  return fRead.eof();
	  }

	  inline void writeEndl() { fWrite << std::endl; }

	  void writeLine(const std::string &buf) {
         try{
            fWrite << buf;
		 }catch(...){
			 throw std::string("cannot write to the file");
		 }
	  }

	  inline void closeRead(){ if(fRead.is_open()) fRead.close(); }
	  inline void closeWrite(){ if(fWrite.is_open()) fWrite.close(); }

public:

	FileUncompress(const int bufsize_ = 1024000) :
	  bufsize(bufsize_)
	  {}


};

}; //end of uim namespace

#endif

