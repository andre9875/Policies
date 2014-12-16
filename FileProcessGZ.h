#ifndef _FILE_POLICE_GZ_H_
#define _FILE_POLICE_GZ_H_

#ifdef __GNUC__

#include <iostream>
#include <string>
#include <stdio.h>


class ReadCompress{
private:

   ReadCompress& operator=(const ReadCompress&){} //disable assigment
   ReadCompress(const ReadCompress&){}            //disable copy ctor

protected:

	int bufsize;
	char *buffer;
	FILE *fRead;
	FILE *fWrite;

	  ~ReadCompress() { delete[] buffer; } //the destructor is protected, only derived classes can destroy policy object

	  void openRead(const std::string &filename,std::ios_base::openmode m_mode = std::ios_base::in){
        try{
	      fRead = popen(filename, m_mode); //Create file.
		  if ( fRead == 0 ) throw std::string("cannot open file");
		}catch(...){
			throw std::string("cannot create file");
		}
	  }

	  void openWrite(const std::string &filename,std::ios_base::openmode m_mode = std::ios_base::ate|std::ios_base::out){
        try{
	      fWrite = popen(filename, m_mode); //Create file.
		  if ( fWrite == 0 ) throw std::string("cannot open file");
		}catch(...){
			throw std::string("cannot create file");
		}
	  }

	  bool readLine(std::string &buf){
		  try{
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

	  inline void closeRead(){ pclose(fRead); }
	  inline void closeWrite(){ pclose(fWrite); }

public:

	ReadCompress(int bufsize_ = 512) :
	  bufsize(bufsize_),
	  buffer(new char[bufsize])
	  {}


};

#endif

#endif
