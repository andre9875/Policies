#ifndef _PROCESS_FILE_
#define _PROCESS_FILE_

//#include "FilePolicyGZ.h"
#include "FilePolicy.h"
#include "SplitPolicy.h"


namespace uim{

template <class FilePolicy, class SplitPolicy>
class ProcessFile : protected FilePolicy, protected SplitPolicy{
private:

	using SplitPolicy::split;
	using FilePolicy::readLine;
	using FilePolicy::writeLine;
	using FilePolicy::writeEndl;
	using FilePolicy::closeRead;
	using FilePolicy::closeWrite;
	using FilePolicy::openRead;
	using FilePolicy::openWrite;

public:

	ProcessFile(int bufsize_ = 1024000) : FilePolicy(bufsize_) {}

	void OpenRead(const std::string &filename, std::ios_base::openmode m_mode = std::ios_base::in) {
	  openRead(filename,m_mode);
	}

	void OpenWrite(const std::string &filename, std::ios_base::openmode m_mode = std::ios_base::ate|std::ios_base::out) {
	  openWrite(filename,m_mode);
	}

    bool ReadLine(std::string &buf){ return readLine(buf); }
	void WriteLine(const std::string &buf){ writeLine(buf); }
	void WriteEndl() { writeEndl(); }
	void CloseRead() { closeRead(); }
	void CloseWrite() { closeWrite(); }
	void Split(const char* val, splitVec &out, const char *delim = "Æ", int reserve_size = 90){
		split(val, out, delim, reserve_size);
	}

};

}; //end of uim namespace

#endif
