#ifndef nxm_RUSAGE_H
#define nxm_RUSAGE_H

#include <iostream>
#include <sys/resource.h>
#include <time.h>
#include <stdexcept> // std::runtime_error
#include <unistd.h>


#ifdef __linux__

#include <glibtop.h>
#include <glibtop/cpu.h>
#include <glibtop/mem.h>
#include <glibtop/proctime.h>
#include <glibtop/procmem.h>

#endif

namespace nxm_rusage{

 class nxm_Rusage
  {
   public:
    nxm_Rusage ();
    nxm_Rusage (int);
   ~nxm_Rusage ();

   #ifdef __linux__
    double cpu_perc_linux();
    double cpu_perc_linux_all();
    long mem_usage_pid_linux();
    long mem_avail_linux();
    long mem_total_linux();
    double mem_usage_perc_linux();
   #endif

   #ifdef __SUNPRO_CC
    double cpu_perc_sun();
   #endif

   private:
    //time_t start,end;

    #ifdef __linux__
      glibtop_cpu cpu_begin,cpu_end;                   /////////////////////////////
      glibtop_proc_time proctime_begin,proctime_end;   ///Declare the CPU info and
      glibtop_mem memory;                              ///memory info struct
      glibtop_proc_mem process_mem;                    ///////////////////////////////
    #endif

    pid_t pid;      // keep process ID
    int interval;  // set interval for CPU
    int _kilobyte;

  };

};
#endif

