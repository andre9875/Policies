#ifndef _ReadLookup_
#define _ReadLookup_

#include "FilePolicy.h"
#include "SplitPolicy.h"
#include <map>
#include <string>
#include <vector>
#include <boost/move/move.hpp>

namespace uim{

typedef std::vector<std::string> vecStr;
typedef std::map<std::string,vecStr> mapVec;
typedef std::map<std::string,std::string> mapStr;

template <class FilePolicy, class SplitPolicy>
class ReadLookup : protected FilePolicy, protected SplitPolicy{
private:

	void OpenRead(const std::string &filename, std::ios_base::openmode m_mode = std::ios_base::in) {
	  FilePolicy::openRead(filename,m_mode);
	}

    bool ReadLine(std::string &buf){ return FilePolicy::readLine(buf); }
	void CloseRead() { FilePolicy::closeRead(); }
	void Split(const char* val, vecStr &out, const char *delim = "Æ", int reserve_size = 90){
		SplitPolicy::split(val, out, delim, reserve_size);
	}

	ReadLookup(int bufsize_ = 1024000) : FilePolicy(bufsize_) {}

public:

    static void getLookup(mapVec &out, std::string &filename,const char *delim = "##", int bufsize = 1024000, int reserve_size = 90) {

	  try{

			/*-----------------------------------------------------------------------------------
			# 0		type		:	video
			# 1		id			:	289587
			# 2		title		:	Tremendo guiso: Trabajo
			# 3		channelId	:	10554
			# 4		siteId		:	112
			# 5		sourceId	:	1684
			# 6		language	:	es
			# 7		shortTitle	:	Trabajo parcial
			# 8		friendlyKey	:	tremendo-guiso-trabajo
			# 9		pubDate		:	2/20/2013 5:44
			# 10	expDate		:	2/20/2018 5:30
			# 11	location	:	130220_2787940_Tremendo_guiso
			# 12	filename	:	2787940
			# 13	duration	:	356
			# 14	providerID	:	2787940
			# 15	uimID		:	UNASSIGNED
			# 16	longform	:	0|1
			# 17	genre		:	6
			# 18	type 		:	channel|MCM
			# 19	channel		:	WLII
			# 20	subchannel	:	SHOWS
			# 21	section		:	MANANA
			# 22	category	:	local TV
			# 23	show		:	show name
			#-----------------------------------------------------------------------------------*/

		     ReadLookup obj(bufsize);
		     std::string buf;
		     vecStr vec;
		     obj.OpenRead(filename);
             while(!obj.ReadLine(buf)){
				 if(buf.at(0) != '-' && buf.at(0) != ' ' ){
				   obj.Split(buf.c_str(),vec,delim,reserve_size);

					if(vec.at(14) == "UNASSIGNED" ){
					  out.insert(std::make_pair(vec.at(15),vecStr(vec))); //copy existing vector to map
					}else{
					  out.insert(std::make_pair(vec.at(14),vecStr(vec))); //copy existing vector to map
					}

				    out.insert(std::make_pair(vec.at(1),boost::move(vec))); //move existing vector to map
			     }
			 }

			 obj.CloseRead();

	  }catch(const char* e){
		  throw std::string(e);
      }catch (const std::out_of_range& oor) {
		  throw std::string(oor.what());
	  }catch(...){
		  throw std::string("generic error");
	  }

	}

};




}; //end of uim namespace

#endif
