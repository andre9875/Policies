#ifndef _FILE_POLICE_PRT_H_
#define _FILE_POLICE_PTR_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <utility>      // std::swap


namespace nxm{

	class File{
	private:

		File& operator=(const File&); //disable assigment
		//File(const File&);            //disable copy ctor

		std::fstream *ptrFile;
		const int bufsize;

	protected:

		~File() { std::cout << this << ": dtor \n"; } //the destructor is protected, only derived classes can destroy policy object

		File(const File& other, const int bufsize_ = 1024000) : bufsize(bufsize_){
		   std::cout << this << ": copy ctor \n";
		   this->ptrFile = other.ptrFile;
		   std::cout << ptrFile << std::endl;
		}

		File(File&& other, const int bufsize_ = 1024000) : bufsize(bufsize_){
		   std::cout << this <<  ": move ctor \n";
		   this->ptrFile = other.ptrFile;
		}


			void openRead(const std::string &filename, std::ios_base::openmode m_mode = std::ios_base::in){
				try{
					ptrFile->open(filename.c_str(), m_mode); //Create file.
					if (!ptrFile->is_open()) throw std::string("cannot open file");
				}
				catch (...){
					throw std::string("cannot create file");
				}
			}

			bool readLine(std::string &buf){
				try{
					char buffer[bufsize];
					ptrFile->getline(buffer, bufsize);
					buf = std::string(buffer);
				}
				catch (...){
					return false;
				}
				return ptrFile->eof();
			}

			void openWrite(const std::string &filename, std::ios_base::openmode m_mode = std::ios_base::ate | std::ios_base::out){
				try{
					ptrFile->open(filename.c_str(), m_mode); //Create file.
					if (!ptrFile->is_open()) throw std::string("cannot open file");
				}
				catch (...){
					throw std::string("cannot create file");
				}
			}

			inline void writeEndl() { *ptrFile << std::endl; }

			void writeLine(const std::string &buf) {
				try{
					*ptrFile << buf;
				}
				catch (...){
					throw std::string("cannot write to the file");
				}
			}

			inline void closeFile(){ if (ptrFile->is_open()) ptrFile->close(); }


	public:

		File(std::fstream *_ptrFile, const int bufsize_ = 1024000) :
			ptrFile(_ptrFile),
			bufsize(bufsize_)
		{
			std::cout << this << ": ctor \n";
		}


		};

	}; //end of nxm namespace

#endif

