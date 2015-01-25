#ifndef UIM_SPLITLIB_H
#define UIM_SPLITLIB_H

//standart libraries
#include <string>
#include <iostream>
#include <exception>
#include <vector>
//#include <boost/move/move.hpp>

//only for test efficiency
//#include <boost/date_time/posix_time/posix_time.hpp>

namespace nxm_split{

typedef std::vector<std::string> _VectorTypeStr;
typedef std::vector<int> _VectorTypeInt;

 class nxmSplit
 {
  public:

   nxmSplit() {}
   ~nxmSplit() {}

   void split_get_key(_VectorTypeStr* , const char* , const char*, const _VectorTypeInt&, std::string&);
   void split_get_key_uu(_VectorTypeStr* , const char* , const char*, const _VectorTypeInt&, std::string&);
   void split_get_key_video(_VectorTypeStr* , const char* , const char*, const _VectorTypeInt&, std::string&,int, int, int);
   void split(_VectorTypeStr* , const char* , const char* );
   void split_newline(_VectorTypeStr* , const char* , const char* );

  private:

   //Disable copy constructor and assignment operator
   nxmSplit(const nxmSplit&); //{}
   nxmSplit& operator=(const nxmSplit&); //{}

 };


void nxm_split::nxmSplit::split_get_key(_VectorTypeStr* tokenList, const char* val, const char *delim, const _VectorTypeInt &_summary_key, std::string &_key){


  try{

// boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::local_time();

  tokenList->clear();
  int start = -1;
  int index = 0;
  int i;

  tokenList->reserve(20);

  for (i = 0; val[i]; ++i){
    if (val[i] == *delim){
     const std::string tmp_val(val + start + 1, val + i);
     tokenList->push_back( std::move(tmp_val) );

     if(_summary_key.at(index) != -1)
       _key += tokenList->at(index) + delim;

     start = i;
     ++index;
    }
  }

  //--- add last token to the vector and remove newline ---
  const std::string tmp_val(val + start + 1, val + (i-1));
  tokenList->push_back( std::move(tmp_val) );

   if(_summary_key.at(index) != -1)
     _key += tokenList->at(index);
   else
     _key.resize(_key.length() - 1);


   // boost::posix_time::ptime end_time = boost::posix_time::microsec_clock::local_time();
   //  std::cout << "split =" << boost::posix_time::to_simple_string(end_time - start_time) << std::endl;

  }catch(...){

	 _key="";
	 tokenList->clear();

  }

}

void nxm_split::nxmSplit::split_get_key_uu(_VectorTypeStr* tokenList, const char* val, const char *delim, const _VectorTypeInt &_summary_key, std::string &_key){


  try{

// boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::local_time();

  tokenList->clear();
  int start = -1;
  int index = 0;
  int i;

  tokenList->reserve(20);

  for (i = 0; val[i]; ++i){
    if (val[i] == *delim){
     const std::string tmp_val(val + start + 1, val + i);
     tokenList->push_back( std::move(tmp_val) );

     if(_summary_key.at(index) != -1)
       _key += tokenList->at(index) + delim;

     start = i;
     ++index;
    }
  }

    //--- add last token to the vector and remove newline ---
     const std::string tmp_val(val + start + 1, val + i);
     tokenList->push_back( std::move(tmp_val) );

    if(_summary_key.at(index) != -1)
     _key += tokenList->at(index);
    else
     _key.resize(_key.length() - 1);


   // boost::posix_time::ptime end_time = boost::posix_time::microsec_clock::local_time();
   //  std::cout << "split =" << boost::posix_time::to_simple_string(end_time - start_time) << std::endl;

  }catch(...){

	 _key="";
	 tokenList->clear();

  }

}



/* ----------------Parameters-------------------------
1 : vector to holds splitted values from a string
2 : string to be splitted
3 : delimiter
4 : vector to hold indexes of summary keys
5 : value of summary key to be returned
6 : position of media type
7 : position if country
8 : _newline(0 default): 0 without new line, 1 with new line
------------------------------------------------------*/
void nxm_split::nxmSplit::split_get_key_video(_VectorTypeStr* tokenList, const char* val, const char *delim, const _VectorTypeInt &_summary_key, std::string &_key, int _type, int _cr, int _newline=0){

//--ONLY USE WITH UIM_VIDEO_LIB SHARED LIBRARY--

  try{

// boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::local_time();

  tokenList->clear();
  int start = -1;
  int index = 0;
  int i;

  tokenList->reserve(20);

  for (i = 0; val[i]; ++i){

    if (val[i] == *delim){

	 if(index==_type){

		 //set MEDIA_TYPE to AD if not equal C or O (Overlay) or L (livestream)
		 if(val[i-1]=='C' || val[i-1]=='c'){
			 tokenList->push_back(std::move("C"));
	     }else if(val[i-1]=='O' || val[i-1]=='o'){
			 tokenList->push_back(std::move("O"));
		 }else if(val[i-1]=='L' || val[i-1]=='l'){
			 tokenList->push_back(std::move("L"));
	     }else{
		     tokenList->push_back(std::move("A"));
	     }

     }else if(index==_cr){

         //set COUNTRY_RES to 1 if US&PR and 0 to rest of countries
		 if( (val[i-3]=='2' && val[i-2]=='2' && val[i-1]=='3') || (val[i-3]=='1' && val[i-2]=='7' && val[i-1]=='2') ){
		   tokenList->push_back(std::move("1"));
		 }else{
		   tokenList->push_back(std::move("0"));
         }

	 }else{
       const std::string tmp_val(val + start + 1, val + i);
       tokenList->push_back( std::move(tmp_val) );
     }

     if(_summary_key.at(index) != -1)
       _key += tokenList->at(index) + delim;

     start = i;
     ++index;
    }

  }

    //--- add last token to the vector ---
    //--- _newline = 0 : without new line
    //--- _newline = 1 : with new line
    if(_newline==0){
     const std::string tmp_val(val + start + 1, val + i);
     tokenList->push_back( std::move(tmp_val) );
    }else{
     const std::string tmp_val(val + start + 1, val + (i-1));
     tokenList->push_back( std::move(tmp_val) );
    }


    if(_summary_key.at(index) != -1)
     _key += tokenList->at(index);
    else
     _key.resize(_key.length() - 1);


   // boost::posix_time::ptime end_time = boost::posix_time::microsec_clock::local_time();
   //  std::cout << "split =" << boost::posix_time::to_simple_string(end_time - start_time) << std::endl;

  }catch(...){

	 _key="";
	 tokenList->clear();

  }

}



void nxm_split::nxmSplit::split_newline(_VectorTypeStr* tokenList, const char* val, const char *delim )
{



 try{

  //boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::local_time();

  tokenList->clear();
  int start = -1;
  int i;

  tokenList->reserve(20);

  for (i = 0; val[i]; ++i){
    if (val[i] == *delim){
     const std::string tmp_val(val + start + 1, val + i);
     tokenList->push_back( std::move(tmp_val) );
     start = i;
    }
  }

  //--- add last token to the vector and remove newline ---
  const std::string tmp_val(val + start + 1, val + (i-1));
  tokenList->push_back( std::move(tmp_val) );

  //boost::posix_time::ptime end_time = boost::posix_time::microsec_clock::local_time();
  //std::cout << "split =" << boost::posix_time::to_simple_string(end_time - start_time) << std::endl;

  }catch(...){
	 tokenList->clear();
  }

}


void nxm_split::nxmSplit::split(_VectorTypeStr* tokenList, const char* val, const char *delim )
{



 try{

  //boost::posix_time::ptime start_time = boost::posix_time::microsec_clock::local_time();

  tokenList->clear();
  int start = -1;
  int i;

  tokenList->reserve(20);

  for (i = 0; val[i]; ++i){
    if (val[i] == *delim){
     const std::string tmp_val(val + start + 1, val + i);
     tokenList->push_back( std::move(tmp_val) );
     start = i;
    }
  }

  //--- add last token to the vector and remove newline ---
  const std::string tmp_val(val + start + 1, val + i);
  tokenList->push_back( std::move(tmp_val) );

  //boost::posix_time::ptime end_time = boost::posix_time::microsec_clock::local_time();
  //std::cout << "split =" << boost::posix_time::to_simple_string(end_time - start_time) << std::endl;

  }catch(...){
	 tokenList->clear();
  }


}


};
#endif


