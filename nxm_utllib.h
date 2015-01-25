#ifndef NXM_UTLLIB_H
#define NXM_UTLLIB_H

#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "nxm_timelib.h"
#include "sendmail.h"

//#include <boost/tokenizer.hpp>

namespace nxm_utl{

 //typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

 class nxm_Utl
 {
  public:
    void split(std::vector<std::string>*, const std::string*, const char*);
    void split(std::vector<std::string>*, char*, const char*);
    void split_newline(std::vector<std::string>*, char*, const char*);
    void split(std::vector<std::string>*, const std::string*, const char);
    void split(std::vector<std::string>*, const std::string*, const std::string*);
    void chomp(std::string&);
    std::string u_case(const std::string*);
    std::string l_case(const std::string*);
    std::string intToString(int);
    std::string longToString(long);
    std::string doubleToString(double);
    int StringToInt(std::string*);
    int StringToInt(std::string &);
    int StringToInt(const char*);
    double StringToDouble(std::string *);
    double StringToDouble(std::string &);
    double StringToDouble(const char*);
    long StringToLong(std::string *);
    long StringToLong(std::string &);
    long StringToLong(const char*);
    int CharsToInt(const char*);
    bool compareString(const std::string&, const std::string&);
    bool compareString_u_case(const std::string&, const std::string&);
    void load_profile(std::map<std::string,std::string>*, const std::string*);
    void load_profile(std::map<std::string,int>&, const std::string&);
    void load_profile(std::map<std::string,long>&, const std::string&);
    long mem_free_bytes(void);
    int mem_free(void);
    int mem_total(void);
    int mem_used(void);
    int pid_mem_usage(int);
    double pid_cpu_usage(pid_t&);
    int get_pid();
    std::string get_pwd(void);
    long get_file_size(std::string);
    long get_file_size(const char*);
    bool check_pid(std::string &);
    bool check_running_pid(std::string &);
    int get_running_pid(std::string &);
    bool fileExists(const std::string&);
    bool open_proc_status(const std::string&, const std::string&);
    std::string basename_wo_ext(std::string &);
    std::string basename(std::string &);
    std::string display_with_commas(int);
    std::string get_path(std::string &);
    std::string get_path(const char*);
 };

};
#endif
