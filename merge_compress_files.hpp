/*
 * merge_compress_files.hpp
 *
 *  Created on: Dec 19, 2014
 *      Author: anedospe
 */

#ifndef MERGE_COMPRESS_FILES_HPP_
#define MERGE_COMPRESS_FILES_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "SplitPolicy.h"
#include "FilePolicyGZ.h"
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

typedef std::vector<SortFiles> vecFiles;
typedef uim::ProcessFile<uim::FileCompress,SplitPolicy> procFile;

void merge_compress_files(const std::string &file1, const std::string &file2, const std::string &fileout){

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
			//std::cout << "[" << buf1 << "]" << std::endl;
			objWriteFile.WriteLine(buf1);
			//objWriteFile.WriteEndl();
		    if(objReadFile1.ReadLine(buf1)) break;
		}else{
			objWriteFile.WriteLine(buf2);
			//objWriteFile.WriteEndl();
	        if(objReadFile2.ReadLine(buf2)) break;
		}
	}

	if(buf1 != ""){
		objWriteFile.WriteLine(buf1);
		//objWriteFile.WriteEndl();
	}

	if(buf2 != ""){
		objWriteFile.WriteLine(buf2);
		//objWriteFile.WriteEndl();
	}

	while(!objReadFile1.ReadLine(buf1)){
		objWriteFile.WriteLine(buf1);
		//objWriteFile.WriteEndl();
	}

	while(!objReadFile2.ReadLine(buf2)){
		objWriteFile.WriteLine(buf2);
		//objWriteFile.WriteEndl();
	}

	objWriteFile.CloseWrite();
	objReadFile1.CloseRead();
	objReadFile2.CloseRead();

}

void build_list_files(const std::string &dir,
		              const std::string &file_pattern,
		              const std::string &fileName,
		              const std::string &fileOut,
		              vecFiles &mergeFiles,
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
				vecFiles.push_back(fileName + std::to_string(vec_num_files.at(i)) + "_" + std::to_string(k) +  ".dat.gz");

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



#endif /* MERGE_COMPRESS_FILES_HPP_ */
