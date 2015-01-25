#ifndef _PROCESS_FILE_
#define _PROCESS_FILE_


namespace nxm{

template <class FilePolicy, class SplitPolicy>
class ProcessFile : protected FilePolicy, protected SplitPolicy{
private:

	using SplitPolicy::split;
	using FilePolicy::readLine;
	using FilePolicy::writeLine;
	using FilePolicy::writeEndl;
	using FilePolicy::closeFile;
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
	void CloseFile() { closeFile(); }
	void Split(const char* val, splitVec &tokenList, const char *delim = "\341\232\226", int reserve_size = 90){
		split(val, tokenList, delim, reserve_size);
	}
	
	void Split(bool flagCSV, const char* val, splitVec &tokenList, const char *delim = "\u002C", const char *enclosure = "\u0022", int reserve_size = 90){
		split(flagCSV, val, tokenList, delim, enclosure, reserve_size);
	}
        
    void Split(const char* val, splitMap &tokenList, const char *delim = ",", const char *link ="=>", const char *ignore = "\"" ){
        split(val,tokenList,delim,link,ignore);
    }
};

}; //end of nxm namespace

#endif
