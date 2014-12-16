#ifndef _DIR_POLICY_
#define _DIR_POLICY_

#include <vector>
#include "dirent.h"
#include "SearchPattern.h"
//#include <boost/move/move.hpp>

namespace uim{

typedef std::vector<std::string> dirVec;

class DirPolicy{
private:
    DIR*     dir;
    dirent*  pdir;
public:

 void readDir(const char *dirname, const std::string &pat, dirVec &vec){

	 SearchPattern sp;
     dir = opendir(dirname);
     if (dir != NULL) {
       while ( (pdir = readdir(dir)) ) {
		   if(sp.str_search(std::string(pdir->d_name),pat)>0){
             vec.push_back(std::string(pdir->d_name));
	       }
       }
     }
 }

};

}; //end of uim namespace

#endif

