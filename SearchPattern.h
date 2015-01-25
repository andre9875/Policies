#ifndef _SearchPattern_h_
#define _SearchPattern_h_

#include <string>
//#include <boost/move/move.hpp>

namespace uim{


class SearchPattern{

private:

	unsigned int SDBMHash(const std::string& str){
		unsigned int hash = 0;
		for(std::size_t i = 0; i < str.length(); i++)
			hash = str[i] + (hash << 6) + (hash << 16) - hash;

		return hash;
	}

public:


	int str_search(const std::string &in, const std::string &pat){

		unsigned int pat_num = SDBMHash(pat);
		unsigned int pat_len = pat.size();
		for(unsigned int i = 0; i < in.size(); i++)
			if(pat_num == (SDBMHash(std::string(in,i,pat_len))) ) return i;

		return 0;

	}

	std::string substr_first_of(const std::string &in, const std::string &pat1){

        unsigned int pos = str_search(in,pat1);
        if(pos == 0) return "0";
        std::string tmp_str( in, pos, pat1.size() );
		return std::move(tmp_str);

	}

	std::string substr_between(const std::string &in, const std::string &pat1, const std::string &pat2){

        unsigned int pos1 = str_search(in,pat1);
        if(pos1 == 0) return "0";
		unsigned int pos2 = str_search(in,pat2);
        if(pos2 == 0) return "0";
		std::string tmp_str(in, pos1 + pat1.size(), pos2 - (pos1 + pat1.size()) );
		return std::move(tmp_str);

	}

};

}; //end of uim namespace

#endif
