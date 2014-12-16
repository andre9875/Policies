#ifndef _FILE_POLICE_GZ_H_
#define _FILE_POLICE_GZ_H_

#ifdef __GNUC__

#include <iostream>
#include <string>
#include <stdio.h>


namespace uim{

class FileCompress{
private:

   FileCompress& operator=(const FileCompress&); //disable assigment
   FileCompress(const FileCompress&);            //disable copy ctor

protected:
	FILE *fRead;
	FILE *fWrite;
	int bufsize;

	  //~FileCompress() { delete[] buffer; } //the destructor is protected, only derived classes can destroy policy object

	  void openRead(const std::string &filename,std::ios_base::openmode m_mode = std::ios_base::in){
        try{
		  std::string cmd = "gunzip -c " + filename;
	      fRead = popen(cmd.c_str(), "r"); //Create file.
		  if ( fRead == 0 ) throw std::string("cannot open file");
		}catch(...){

			throw std::string("cannot create file");
		}
	  }

	  void openWrite(const std::string &filename,std::ios_base::openmode m_mode = std::ios_base::ate|std::ios_base::out){
        try{
		  std::string cmd = "gzip > " + filename;
	      fWrite = popen(cmd.c_str(), "w"); //Create file.
		  if ( fWrite == 0 ) throw std::string("cannot open file");
		}catch(...){
			throw std::string("cannot create file");
		}
	  }

	  bool readLine(std::string &buf){

		   try{
			char buffer[bufsize];
		    fgets( buffer, bufsize, fRead );
		    buf = std::string(buffer);
		  }catch(...){
			  return false;
		  }
		  return feof(fRead);
	  }

	  inline void writeEndl() { fprintf(fWrite, "%s", "\n" ); }

	  void writeLine(const std::string &buf) {
         try{
			fprintf(fWrite, "%s", buf.c_str() );
		 }catch(...){
			 throw std::string("cannot write to the file");
		 }
	  }

	  inline void closeRead(){ if ( fRead != 0 ) pclose(fRead); }
	  inline void closeWrite(){ if ( fWrite != 0 ) pclose(fWrite); }

public:

	FileCompress(int bufsize_ = 1024000) :
	  bufsize(bufsize_),
	  fRead(0),
	  fWrite(0)
	  {}


};

}; //end of uim namespace

#endif

#endif

