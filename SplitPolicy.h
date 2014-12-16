#ifndef _SPLIT_POLICY_
#define _SPLIT_POLICY_

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <exception>

#ifdef __GNUC__
#include <boost/move/move.hpp>
#endif

typedef std::vector<std::string> splitVec;

class SplitPolicy {
private:
	SplitPolicy(const SplitPolicy&);
	SplitPolicy& operator=(const SplitPolicy&);

protected:

void splitCSV(const char* val, splitVec &out,int reserve_size = 90, char comma = ',', char quota = '"'){

	out.clear();
	out.reserve(reserve_size);

	int total_size = strlen(val);
	int start = -1;
	int i;

	bool newLine = false;
	if(val[total_size-1] == '\n')
	   newLine = true;


	try{

		for (i = 0; val[i]; ++i){


			if (val[i] == comma && val[i + 1] == quota){ //Case where next node begins with " (double quota)

				if (val[start] == comma){ //Case where previous node begins with , (comma)
					std::string l_val(val + start + 1, val + i);
					//std::cout << l_val << std::endl;

#ifdef __GNUC__
					out.push_back(boost::move(l_val));
#else
					out.push_back(l_val);
#endif

				}


				if (start == -1){ //Case where first node begins without " (double quota)
					std::string l_val(val + start + 1, val + i);

#ifdef __GNUC__
					out.push_back(boost::move(l_val));
#else
					out.push_back(l_val);
#endif

				}

				int k = ++i;
				start = ++k;
				while (true){ //val[k] != quota && val[k + 1] != quota){

					if (val[k] == quota && val[k + 1] == quota){
						k += 2;
					}
					else{
						++k;
					}


					if (val[k] == quota && val[k + 1] == comma)
						break;
					else if (k == total_size-1)
						break;

				}

				std::string l_val(val + start, val + k);
				l_val="\"" + l_val + "\"";
				//std::cout << l_val << std::endl;

#ifdef __GNUC__
				out.push_back(boost::move(l_val));
#else
				out.push_back(l_val);
#endif

				i = k;

			}else if (i==0 && val[0] == quota){ //Case where first node begins with " (double quota)

					int k = ++i;
					start = k;
					while (true){ //val[k] != quota && val[k + 1] != quota){

						if (val[k] == quota && val[k + 1] == quota){
							k += 2;
						}
						else{
							++k;
						}

						if (val[k] == quota && val[k + 1] == comma)
							break;
						else if (k == total_size-1)
							break;

					}

					std::string l_val(val + start, val + k);
                    l_val="\"" + l_val + "\"";
					//std::cout << l_val << std::endl;

#ifdef __GNUC__
					out.push_back(boost::move(l_val));
#else
					out.push_back(l_val);
#endif

					i = k;

			}else if (val[i] == comma){ //Case where next node begins with , (comma)

				std::string l_val;
				if (val[i] == comma && val[i - 1] != quota){
					l_val = std::string(val + start + 1, val + i);

					//std::cout << l_val << std::endl;

#ifdef __GNUC__
					out.push_back(boost::move(l_val));
#else
					out.push_back(l_val);
#endif

				}

				start = i;
			}

		}// end of for loop

		if (i == total_size){

			if (val[start] == comma){

			  //--- add last token to the vector ---
			  if(newLine == false){
				//const std::string tmp_val(val + start + lenDelim, val + i);
				std::string l_val(val + start + 1, val + i);
				out.push_back( boost::move(l_val) );
			  }else{
				//const std::string tmp_val(val + start + lenDelim, val + (i-1));
				std::string l_val(val + start + 1, val + (i-1));
				out.push_back( boost::move(l_val) );
			  }


/*
				std::string l_val(val + start + 1, val + i);
				//std::cout << l_val << std::endl;
#ifdef __GNUC__
				out.push_back(boost::move(l_val));
#else
				out.push_back(l_val);
#endif

*/

			}
		}

		//cout << "i=" << i << " : start=" << start << " : total_size=" << total_size <<  endl;

	}
	catch (...){
		throw std::string("split error");
	}


}


void split(const char* val, splitVec &tokenList, const char *delim = "Æ", int reserve_size = 90 )
{


	try{

		tokenList.clear();
		int i;
		bool newLine = false;
		if (val[strlen(val) - 1] == '\n')
			newLine = true;

		tokenList.reserve(20);

		int lenDelim = strlen(delim);
		int start = -1 * lenDelim;
		short emptyToken = 0;

		for (i = 0; val[i]; ++i){

			bool flagDelim = false;
			for (short d = 0; d < lenDelim; ++d){
				if (val[i + d] == delim[d]){
					flagDelim = true;
					++emptyToken;
				}else{
					flagDelim = false;
					emptyToken = 0;
					break;
				}
			}


			if (flagDelim == true){

				if (emptyToken > lenDelim){
					tokenList.push_back("");
					i += (lenDelim - 1);
					start = i;
				}else{
					const std::string tmp_val(val + start + lenDelim, val + i);
					tokenList.push_back(boost::move(tmp_val));
					//tokenList.push_back(std::move(tmp_val));
                    start = i;
				}

			}

		}

		//--- add last token to the vector ---
		if (newLine == false){
			const std::string tmp_val(val + start + lenDelim, val + i);
			tokenList.push_back(boost::move(tmp_val));
			//tokenList.push_back(std::move(tmp_val));
		}else{
			const std::string tmp_val(val + start + lenDelim, val + (i - 1));
			//tokenList.push_back(std::move(tmp_val));
			tokenList.push_back(boost::move(tmp_val));
		}


	}
	catch (...){
		tokenList.clear();
	}


}

public:
	SplitPolicy() {}
};

#endif
