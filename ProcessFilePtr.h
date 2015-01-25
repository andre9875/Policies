#ifndef _PROCESS_FILE_
#define _PROCESS_FILE_

#include <fstream>

namespace nxm{

template <class FilePolicy, class SplitPolicy>
class ProcessFilePtr : protected FilePolicy, protected SplitPolicy{
private:

	using SplitPolicy::split;
	using FilePolicy::readLine;
	using FilePolicy::writeLine;
	using FilePolicy::writeEndl;
	using FilePolicy::closeFile;
	using FilePolicy::openRead;
	using FilePolicy::openWrite;

public:

	ProcessFilePtr(std::fstream *_ptrFile, int bufsize_ = 1024000) : FilePolicy(_ptrFile, bufsize_) {}

	void OpenRead(const std::string &filename, std::ios_base::openmode m_mode = std::ios_base::in) {
	  openRead(filename,m_mode);
	}

	void OpenWrite(const std::string &filename, std::ios_base::openmode m_mode = std::ios_base::ate|std::ios_base::out) {
	  openWrite(filename,m_mode);
	}

    bool ReadLine(std::string &buf){ return readLine(buf); }
	void WriteLine(const std::string &buf){ writeLine(buf); }
	void WriteEndl() { writeEndl(); }
	void CloseFile() { closeFile(); }
	void Split(const char* val, splitVec &out, const char *delim = "�", int reserve_size = 90){
		split(val, out, delim, reserve_size);
	}

};

}; //end of nxm namespace

#endif
