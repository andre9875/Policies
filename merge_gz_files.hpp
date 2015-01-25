/*
 * merge_gz_files.hpp
 *
 *  Created on: Jan 6, 2015
 *      Author: anedospe
 */

#ifndef MERGE_GZ_FILES_HPP_
#define MERGE_GZ_FILES_HPP_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "SplitPolicy.h"
#include "FilePolicyGz.h"
#include "ProcessFile.h"
#include "DirPolicy.h"

typedef std::vector<std::string> vecStr;
typedef std::vector<unsigned int> vecInt;
typedef std::vector<vecStr> vecVec;

struct SortFiles{
	std::string file_in_1;
	std::string file_in_2;
	std::string file_out;
};

typedef std::vector<SortFiles> vecSortFiles;
typedef nxm::ProcessFile<nxm::FileGZ,nxm::SplitPolicy> processFile;
typedef std::vector<processFile> vecProcessFile;
//typedef std::vector<std::shared_ptr<std::fstream>> vecFiles;

//forward declaration
void returnPos(const vecStr&,unsigned int&,const std::vector<bool>&);
unsigned int getPos(const std::vector<bool>&);
bool testEOFs(const std::vector<bool> &);


void merge_files(const vecStr &mergeFiles, const std::string &fileout,const std::string &dir){

	processFile FileOut; //write result to the final file
	FileOut.OpenWrite(dir + fileout); //create write file

	//vecFiles filePointers; //keep list of fstream pointers
	vecProcessFile fileInstances; //keep list of assigned FILE pointers
	vecStr compString; //keep value from current open cursor per opened file
	std::vector<bool> eofFlag(mergeFiles.size(),false); //keep track of EOF
	//filePointers.reserve(mergeFiles.size());
	fileInstances.reserve(mergeFiles.size());
	compString.reserve(mergeFiles.size());

	//create pool of file pointers
	for(unsigned int i =0; i < mergeFiles.size(); ++i){
		//filePointers.push_back(std::make_shared<std::fstream>()); //add file pointer to the vector

		{
			processFile tmpFile; //create instance of processFile
			fileInstances.push_back(tmpFile);              //add instance of processFile to the vector
		}
        //std::cout << mergeFiles.at(i) << std::endl;
		fileInstances.at(i).OpenRead(dir + mergeFiles.at(i)); //open file for read
	}

    //read first lines
    std::string buf;
	for(unsigned int i =0; i < mergeFiles.size(); ++i){
		fileInstances.at(i).ReadLine(buf); //read line from file
		compString.push_back(std::move(buf)); //move value from each file to vector
	}

	unsigned int posVal(0); //set initial position to the start
	returnPos(compString,posVal,eofFlag); //return position of lowest value
	FileOut.WriteLine(compString.at(posVal)); //append the value to the file
	//FileOut.WriteEndl(); //add new line
	//std::cout << posVal << " : " << compString.at(posVal) << std::endl;
	compString.at(posVal).clear(); //clear appended value

	while(true){ //run unless all files are read

		if(!fileInstances.at(posVal).ReadLine(buf)){ //test if particular file for EOF
		  //std::cout << "posVal=" << posVal << "  buf:"<<buf<<std::endl;
		  compString.at(posVal) = std::move(buf); //move value from each file to vector

		  returnPos(compString,posVal,eofFlag); //return position of lowest value
		  //std::cout << posVal << " : " << compString.at(posVal) << std::endl;

		  FileOut.WriteLine(compString.at(posVal)); //append the value to the file
		  //FileOut.WriteEndl(); //add new line
		  compString.at(posVal).clear(); //clear appended value

		}else{
			eofFlag.at(posVal)=true; //set EOF = true for particular file

			if(testEOFs(eofFlag)) break; //test if any files still open for read

			returnPos(compString,posVal,eofFlag); //return position of lowest value
			//std::cout << posVal << " : " << compString.at(posVal) << std::endl;
			FileOut.WriteLine(compString.at(posVal)); //append the value to the file
			//FileOut.WriteEndl(); //add new line
			compString.at(posVal).clear(); //clear appended value
		}

		if(testEOFs(eofFlag)) break; //test if any files still open for read

	}

	//close all opened files.
	for(unsigned int i = 0; i < fileInstances.size(); ++i){
		fileInstances.at(i).CloseFile();
	}

	FileOut.CloseFile(); //close write file

}


unsigned int getPos(const std::vector<bool> &eofFlag){

	unsigned int i(0);

	for(i=0; i < eofFlag.size(); ++i)
		if(eofFlag.at(i)==false)break;

	return i;
}


void returnPos(const vecStr &compString, unsigned int &posVal,const std::vector<bool> &eofFlag){

	if(eofFlag.at(posVal)==true)
		posVal=getPos(eofFlag);

	if(posVal < compString.size() ){
	  std::string tmp(compString.at(posVal));
	  //std::cout << "tmp: " << tmp << std::endl;


	  for(unsigned int i = 0; i < compString.size(); ++i){

		 //std::cout << i << " : " << compString.at(i) << ": " << compString.at(posVal) <<  std::endl;
         if(eofFlag.at(i)==false){ //test for EOF
		  if(tmp > compString.at(i)){ //compare strings ascending order
			posVal = i; //set position for lowest value
			tmp = compString.at(i); //set lowest value
            //std::cout << posVal << std::endl;
		  }
        }

	  }

	}
}


bool testEOFs(const std::vector<bool> &eofFlag){

	bool _Flag=true;
	for(unsigned int i=0; i < eofFlag.size(); ++i){
		if(eofFlag.at(i)==false){
			_Flag=false;
			break;
		}
	}

	return _Flag;
}

void build_list_files(const std::string &dir,
		              const std::string &file_pattern,
		              vecStr &mergeFiles
					 ){

	uim::DirPolicy objDir;
	objDir.readDir(dir.c_str(),file_pattern,mergeFiles); //read directory

}

void build_list_files(const std::string &dir,
		              const std::string &file_pattern,
		              const std::string &fileName,
		              const std::string &fileOut,
		              vecSortFiles &mergeFiles,
		              vecInt &numFiles
					 ){

	vecInt vec_num_files;
	vecStr vec_files;
	uim::DirPolicy objDir;
	vecVec vec_tmp_files;

	objDir.readDir(dir.c_str(),file_pattern,vec_files); //read directory
	vec_num_files.push_back(vec_files.size());			//get total numbers of files

	//build merge path like 8 -> 4 -> 2 -> final file
	for(unsigned int i = 0; i < vec_num_files.size(); ++i){
		if( (vec_num_files.at(i) / 2) > 1 )
			vec_num_files.push_back((vec_num_files.at(i) / 2));
	}

	numFiles = vec_num_files;

    //build hierarchy of files by level to be merge
	for(unsigned int i = 0; i < vec_num_files.size(); ++i){

		if( i == 0){
			vec_tmp_files.push_back(vec_files);
		}else{
			vecStr vecFiles;
			for(unsigned int k = 0; k < vec_num_files.at(i); ++k)
				vecFiles.push_back(fileName + std::to_string(vec_num_files.at(i)) + "_" + std::to_string(k) +  ".dat");

			vec_tmp_files.push_back(vecFiles);
		}

	}

	unsigned int next_level=1;
	//build list of files by level to be merge
	for(unsigned int i = 0; i < vec_tmp_files.size(); ++i){

        unsigned int next_level_file=0;
        for(unsigned int k = 0; k < vec_tmp_files.at(i).size(); ++k){

        	SortFiles objSortFiles; //temporary file struct
			objSortFiles.file_in_1 = dir + vec_tmp_files.at(i).at(k);
			objSortFiles.file_in_2 = dir + vec_tmp_files.at(i).at(k+1);
			++k;

			if( next_level < vec_tmp_files.size() )
				objSortFiles.file_out = dir + vec_tmp_files.at(next_level).at(next_level_file);
			else
				objSortFiles.file_out = dir + fileOut;

			mergeFiles.push_back(std::move(objSortFiles)); //append file struct to a vector
			++next_level_file;
		}

		++next_level; //increment next level of merged files

	}


}

/*
void merge_two_files(const std::string &file1, const std::string &file2, const std::string &fileout){

	procFile objReadFile1;
	procFile objReadFile2;
	procFile objWriteFile;

	objReadFile1.OpenRead(file1);
	objReadFile2.OpenRead(file2);
	objWriteFile.OpenWrite(fileout);

	std::string buf1;
	std::string buf2;
	short fingerPointer;

	objReadFile1.ReadLine(buf1);
	objReadFile2.ReadLine(buf2);

	while(true){

		if(buf1 < buf2)
			fingerPointer=1;
		else
			fingerPointer=2;

		if(fingerPointer==1){
			objWriteFile.WriteLine(buf1);
			objWriteFile.WriteEndl();
		    if(objReadFile1.ReadLine(buf1)) break;
		}else{
			objWriteFile.WriteLine(buf2);
			objWriteFile.WriteEndl();
	        if(objReadFile2.ReadLine(buf2)) break;
		}
	}

	if(buf1 != ""){
		objWriteFile.WriteLine(buf1);
		objWriteFile.WriteEndl();
	}

	if(buf2 != ""){
		objWriteFile.WriteLine(buf2);
		objWriteFile.WriteEndl();
	}

	while(!objReadFile1.ReadLine(buf1)){
		objWriteFile.WriteLine(buf1);
		objWriteFile.WriteEndl();
	}

	while(!objReadFile2.ReadLine(buf2)){
		objWriteFile.WriteLine(buf2);
		objWriteFile.WriteEndl();
	}

	objWriteFile.CloseWrite();
	objReadFile1.CloseRead();
	objReadFile2.CloseRead();

}
*/

#endif /* MERGE_GZ_FILES_HPP_ */
