#ifndef _FILE_POLICE_GZ_PRT_H_
#define _FILE_POLICE_GZ_PTR_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <utility>      // std::swap


namespace nxm{

	class FileGZ{
	private:

		FileGZ& operator=(const FileGZ&); //disable assigment
		//FileGZ(const FileGZ&);            //disable copy ctor

	FILE  *ptrFile;
	const int bufsize;

	protected:

		~FileGZ() { std::cout << this << ": dtor \n"; } //the destructor is protected, only derived classes can destroy policy object

		FileGZ(const FileGZ& other, const int bufsize_ = 1024000) : bufsize(bufsize_){
		   std::cout << this << ": copy ctor \n";
		   this->ptrFile = other.ptrFile;
		   std::cout << ptrFile << std::endl;
		}

		FileGZ(FileGZ&& other, const int bufsize_ = 1024000) : bufsize(bufsize_){
		   std::cout << this <<  ": move ctor \n";
		   this->ptrFile = other.ptrFile;
		}


			void openRead(const std::string &filename, std::ios_base::openmode m_mode = std::ios_base::in){
				try{
					std::string cmd = "gunzip -c " + filename;
					ptrFile = popen(cmd.c_str(), "r"); //Create file.
					if ( ptrFile == 0 ) throw std::string("cannot open file");
				}catch(...){
					throw std::string("cannot create file");
				}
			}

			bool readLine(std::string &buf){
				try{
					char buffer[bufsize];
					fgets( buffer, bufsize, ptrFile );
					buf = std::string(buffer);
				}catch(...){
					return false;
				}
				return feof(ptrFile);
			}

			void openWrite(const std::string &filename, std::ios_base::openmode m_mode = std::ios_base::ate | std::ios_base::out){
				try{
					std::string cmd = "gzip > " + filename;
					ptrFile = popen(cmd.c_str(), "w"); //Create file.
					if ( ptrFile == 0 ) throw std::string("cannot open file");
				}catch(...){
					throw std::string("cannot create file");
				}
			}

			inline void writeEndl() { fprintf(ptrFile, "%s", "\n" ); }

			void writeLine(const std::string &buf) {
				try{
					fprintf(ptrFile, "%s", buf.c_str() );
				}catch(...){
					throw std::string("cannot write to the file");
				}
			}

			inline void closeFile(){ if ( ptrFile != 0 ) pclose(ptrFile); }


	public:

		FileGZ(const int bufsize_ = 1024000) :
			bufsize(bufsize_)
		{
			std::cout << this << ": ctor \n";
		}


		};

	}; //end of nxm namespace

#endif


