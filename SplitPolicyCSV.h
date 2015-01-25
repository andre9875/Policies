#ifndef _SPLIT_POLICY_CSV_
#define _SPLIT_POLICY_CSV_

#include <iostream>
#include <string>
#include <vector>
#include <exception>

#ifdef __GNUC__
#include <boost/move/move.hpp>
#endif


typedef std::vector<std::string> splitVec;

class SplitPolicyCSV {
private:
	SplitPolicyCSV(const SplitPolicyCSV&);
	SplitPolicyCSV& operator=(const SplitPolicyCSV&);

protected:

	void splitCSV(const char* val, splitVec &out, bool newLine = false, int reserve_size = 90, char comma = ',', char quota = '"'){

		out.clear();
		out.reserve(reserve_size);

		int start = -1;
		int i;
		bool skipFlag = false;
		std::string g_val;

	  try{

		  for (i = 0; val[i]; ++i){

			  if ( val[i] == comma && skipFlag == false && val[i - 1] != quota && val[i + 1] != quota ){
				  std::string l_val(val + start + 1, val + i);

                  #ifdef __GNUC__
				    out.push_back( boost::move(l_val) );
                  #else
				    out.push_back( l_val );
                  #endif

				  start = i;
			  }else if(val[i] == comma && val[i + 1] == quota && val[i - 1] != quota){
				  std::string l_val(val + start + 1, val + i);

				  #ifdef __GNUC__
				    out.push_back( boost::move(l_val) );
                  #else
				    out.push_back( l_val );
                  #endif

				  start = ++i;
				  skipFlag = true;
			  }else if(val[i] == quota && val[i + 1] == comma ){
				  std::string l_val(val + start + 1, val + i);
				  g_val += l_val;

				  #ifdef __GNUC__
				    out.push_back( boost::move(g_val) );
                  #else
				    out.push_back( g_val );
                  #endif

				  g_val="";
				  start = ++i;
				  skipFlag = false;
			  }else if(val[i] == comma && skipFlag == true){
				  std::string l_val(val + start + 1, val + i);
				  g_val += l_val + " ";
                  start = i;
			  }


		  }

		  if(newLine == false){
		    std::string l_val(val + start + 1, val + i);
		    out.push_back( l_val );
	      }else{
            std::string l_val(val + start + 1, val + (i - 1));
		    out.push_back( l_val );
	      }


	  }catch(...){
         throw std::string("split error");
	  }


	}

public:
	SplitPolicyCSV() {}
};

#endif
