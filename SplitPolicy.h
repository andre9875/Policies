#ifndef _SPLIT_POLICY_
#define _SPLIT_POLICY_

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <exception>
#include <utility>

typedef std::vector<std::string> splitVec;

namespace nxm{

class SplitPolicy {
private:
	//SplitPolicy(const SplitPolicy&);
	//SplitPolicy& operator=(const SplitPolicy&);

	bool scanDelimiter(const char* val, const char *delim, int lenDelim, int pos, short &emptyToken){

		for(int d=0; d<lenDelim; ++d){
			if(val[pos + d] == delim[d]){  	//compare delimiter with string
				++emptyToken;        		//keep track of empty spaces between delimiters
			}else{
				emptyToken = 0;
				return false;
			}
		}
		return true;
	}

	bool scanEnclosure(const char* val, const char *enclosure, int lenEnclosure, int pos){

		for(int d=0; d<lenEnclosure; ++d){
			if(val[pos + d] != enclosure[d]){  //compare enclosure with string
				return false;
			}
		}
		return true;
	}

protected:

	void split(const char* val, splitVec &tokenList, const char *delim = "\341\232\226", int reserve_size = 90 ){

			try{
					tokenList.clear();
					int i;
					bool newLine = false;
					if (val[strlen(val) - 1] == '\n') //test for the \n
							newLine = true;

					tokenList.reserve(reserve_size);

					int lenDelim = strlen(delim);	//get length of delimiter
					int start = -1 * lenDelim;		//handle first char in case if it's delimiter
					short emptyToken = 0;			//handle empty space between two delimiters

					for (i = 0; val[i]; ++i){

							if(scanDelimiter(val,delim,lenDelim,i,emptyToken)==true){

									if (emptyToken > lenDelim){ //test if empty space between delimiters
											tokenList.push_back("");
											i += (lenDelim - 1);
											start = i;
									}else{
											const std::string tmp_val(val + start + lenDelim, val + i);
											tokenList.push_back(std::move(tmp_val));
											start = i;
									}
							}
					}

					//--- add last token to the vector ---
					if (newLine == false){
							const std::string tmp_val(val + start + lenDelim, val + i);
							tokenList.push_back(std::move(tmp_val));
					}else{
							const std::string tmp_val(val + start + lenDelim, val + (i - 1));
							tokenList.push_back(std::move(tmp_val));
					}
			}
			catch (...){
					tokenList.clear();
			}
	}


	void split(bool flagCSV, const char* val, splitVec &tokenList, const char *delim = "\u002C", const char *enclosure = "\u0022", int reserve_size = 90){

		tokenList.clear();
		tokenList.reserve(reserve_size);

		int total_size = strlen(val);
		int i;
		int lenDelim = strlen(delim);			//get length of delimeter
		int lenEnclosure = strlen(enclosure);	//get length of enclosure
		int start = -1 * lenDelim;				//handle first char in case if it's delimeter
		short emptyToken = 0;					//handle empty space between two delimeters

		//bool newLine = false;
		if(val[total_size-1] == '\n'){
		  // newLine = true;
		   total_size-=1;
		}

		try{

			for (i = 0; val[i]; ++i){

				//Case where next node begins with Enclosure
				if ( scanDelimiter(val,delim,lenDelim,i,emptyToken) == true && scanEnclosure(val,enclosure,lenEnclosure,(i+lenDelim)) == true ){

					//Case where previous node begins with delimiter
					if ( scanDelimiter(val,delim,lenDelim,start,emptyToken) == true ){
						const std::string l_val(val + start + lenDelim, val + i);
						tokenList.push_back(std::move(l_val));
					//Case where first node begins without Enclosure
					}else if (start == (-1 * lenDelim)){
						const std::string l_val(val + start + lenDelim, val + i);
						tokenList.push_back(std::move(l_val));
					}

					int k = i + lenDelim; 	//update cursor
					k += lenEnclosure;		//update cursor
					start = k;				//update start pointer
					int mid_point = k;		//set pointer for enclosure
					splitVec vecNodes;		//keep temporary nodes

					while (true){
						//current pointer equals enclosure and next pointer equals enclosure
						if (scanEnclosure(val,enclosure,lenEnclosure,k) == true && scanEnclosure(val,enclosure,lenEnclosure,(k + lenEnclosure)) == true ){
							std::string tmp(val + mid_point, val + (k+lenEnclosure));
							vecNodes.push_back(std::move(tmp));
							k += lenEnclosure * 2;
							mid_point = k;
						}else{
							++k;
						}
						//current pointer equals enclosure and next pointer equals delimiter
						if (scanEnclosure(val,enclosure,lenEnclosure,k) == true && scanDelimiter(val,delim,lenDelim,(k + lenEnclosure),emptyToken) == true){
							std::string tmp(val + mid_point, val + k);
							vecNodes.push_back(std::move(tmp));
							break;
						}else if (k == total_size-1){
							std::string tmp(val + mid_point, val + k);
							vecNodes.push_back(std::move(tmp));
							break;
						}
					}// end of while loop

					if(vecNodes.size()>0){
						std::string l_val;
						for(int p=0; p < vecNodes.size(); ++p)
							l_val.append(vecNodes.at(p));
						tokenList.push_back(std::move(l_val));
					}else{
						std::string l_val(val + start, val + k);
						//l_val=enclosure + l_val + enclosure;
						tokenList.push_back(std::move(l_val));
					}

					i = k;

				//Case where first node begins with Enclosure
				}else if (i==0 && scanEnclosure(val,enclosure,lenEnclosure,0) == true){

						int k = i + lenEnclosure;	//update cursor
						start = k;					//update start pointer
						int mid_point = k;			//set pointer for enclosure
						splitVec vecNodes;			//keep temporary nodes

						while (true){
							//current pointer equals enclosure and next pointer equals enclosure
							if (scanEnclosure(val,enclosure,lenEnclosure,k) == true && scanEnclosure(val,enclosure,lenEnclosure,(k + lenEnclosure)) == true ){
								std::string tmp(val + mid_point, val + (k+lenEnclosure));
								vecNodes.push_back(std::move(tmp));
								k += lenEnclosure * 2;
								mid_point = k;
							}else{
								++k;
							}
							//current pointer equals enclosure and next pointer equals delimiter
							if (scanEnclosure(val,enclosure,lenEnclosure,k) == true && scanDelimiter(val,delim,lenDelim,(k + lenEnclosure),emptyToken) == true){
								std::string tmp(val + mid_point, val + k);
								vecNodes.push_back(std::move(tmp));
								break;
							} else if (k == total_size-1){
								std::string tmp(val + mid_point, val + k);
								vecNodes.push_back(std::move(tmp));
								break;
							}

						}

						if(vecNodes.size()>0){
							std::string l_val;
							for(int p=0; p < vecNodes.size(); ++p)
								l_val.append(vecNodes.at(p));
							tokenList.push_back(std::move(l_val));
						}else{
							std::string l_val(val + start, val + k);
							//l_val=enclosure + l_val + enclosure;
							tokenList.push_back(std::move(l_val));
						}

						i = k;

				//Case where next node begins with delimiter
				}else if (scanDelimiter(val,delim,lenDelim,i,emptyToken) == true){

					//Case where first node begins without Enclosure
					if (start == (-1 * lenDelim)){
						const std::string l_val(val + start + lenDelim, val + i);
						tokenList.push_back(std::move(l_val));
					}

					int k = i + lenDelim;
					start = k;
					while (true){
						//current pointer equals delimiter
						if (scanDelimiter(val,delim,lenDelim,k,emptyToken) == true)
							break;
						else if (k == total_size-1){
							++k;
							break;
						}
						++k;
					}

					std::string l_val(val + start, val + k);
					tokenList.push_back(std::move(l_val));

				}// end of if conditions

			}// end of for loop

		}
		catch (...){
			throw std::string("split error");
		}


	}

public:
	SplitPolicy() {}
};

}; //end of nxm namespace

#endif
