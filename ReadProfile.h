#ifndef _ReadProfile_
#define _ReadProfile_

#include "FilePolicy.h"
#include "SplitPolicy.h"
#include <map>
#include <string>
#include <vector>

namespace uim{

typedef std::map<std::string,std::string> mapStr;
typedef std::vector<std::string> vecStr;

template <class FilePolicy, class SplitPolicy>
class ReadProfile : protected FilePolicy, protected SplitPolicy{
private:

	void OpenRead(const std::string &filename, std::ios_base::openmode m_mode = std::ios_base::in) {
	  FilePolicy::openRead(filename,m_mode);
	}

    bool ReadLine(std::string &buf){ return FilePolicy::readLine(buf); }
	void CloseRead() { FilePolicy::closeRead(); }
	void Split(const char* val, splitVec &out, const char *delim = "Æ", int reserve_size = 90){
		SplitPolicy::split(val, out, delim, reserve_size);
	}

    ReadProfile(int bufsize_ = 1024000) : FilePolicy(bufsize_) {}

public:

    static void getProfile(mapStr &out, const std::string &filename,const char *delim = ":", int bufsize = 1024000, int reserve_size = 90) {

	  try{

		     ReadProfile obj(bufsize);
		     std::string buf;
		     vecStr vec;
		     obj.OpenRead(filename);
             while(!obj.ReadLine(buf)){
				 if(buf.at(0) != '-' && buf.at(0) != '#' && buf.at(0) != ' ' ){
				   obj.Split(buf.c_str(),vec,delim,reserve_size);
				   out.insert(std::pair<std::string,std::string>(vec.at(0),vec.at(1)));
			     }
			 }



			 obj.CloseRead();


	  }catch(const char* e){
		  throw std::string(e);
	  }catch(...){
		  throw std::string("generic error");
	  }

	}


};




}; //end of uim namespace

#endif
