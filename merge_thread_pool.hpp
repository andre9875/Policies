/*
 * merge_thread_pool.hpp
 *
 *  Created on: Dec 17, 2014
 *      Author: anedospe
 */

#ifndef MERGE_THREAD_POOL_HPP_
#define MERGE_THREAD_POOL_HPP_

#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>

//#include "merge_files.hpp"
#include "merge_compress_files.hpp"

typedef std::vector<std::thread> vecThread;

class MergeThreadPool{

private:

	const vecFiles *mergeFiles;
	const vecInt *numFiles;
	unsigned int num_threads;
	std::atomic<unsigned int> sortLevel;
	std::atomic<unsigned int> runningThreads;
	std::atomic<unsigned int> fileIndex;
	std::atomic_bool runThread;
	std::atomic_bool isDone;
	std::mutex Mutex;
	std::condition_variable isWorkerAval;
	std::condition_variable isThreadAval;
	vecThread workers;

    void wait_for_level(unsigned int level){

        while(1){
        	if( level == sortLevel.load(std::memory_order_relaxed) ) break;
        	//std::cout << "wait_for_level: " << sortLevel.load(std::memory_order_relaxed) << " : " << std::endl;
        	std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

	void worker_thread(){

       while(runThread.load(std::memory_order_relaxed)){


    	  { // Wait until main loop sends data
            std::unique_lock<std::mutex> lk(Mutex);
            isWorkerAval.wait(lk);
    	  }

    	  runningThreads.fetch_add(1, std::memory_order_relaxed);

          try{

        	if(!isDone.load(std::memory_order_relaxed)){

        	   std::cout << "before=" << fileIndex.load(std::memory_order_relaxed) << std::endl;
               unsigned int inx = fileIndex.fetch_add(1, std::memory_order_relaxed);
               std::cout << "inx=" << inx << std::endl;
               std::cout << "after=" << fileIndex.load(std::memory_order_relaxed) << std::endl;

               /*
		       merge_files(mergeFiles->at(inx).file_in_1,
				           mergeFiles->at(inx).file_in_2,
				           mergeFiles->at(inx).file_out);
			   */

		       merge_compress_files(mergeFiles->at(inx).file_in_1,
				           mergeFiles->at(inx).file_in_2,
				           mergeFiles->at(inx).file_out);

        	}

          }catch(const std::exception& e){
        	  std::cout << e.what() << std::endl;
          }

          //fileIndex.fetch_add(1, std::memory_order_relaxed);

          //std::this_thread::sleep_for(std::chrono::seconds(10));

          runningThreads.fetch_sub(1, std::memory_order_relaxed);
          sortLevel.fetch_add(1, std::memory_order_relaxed);
          isThreadAval.notify_one(); //send note that thread is available

       }

	}

public:

	MergeThreadPool(const vecFiles *_mergeFiles, const vecInt *_numFiles, unsigned int _num_threads=4):
		mergeFiles(_mergeFiles),
		numFiles(_numFiles),
		num_threads(_num_threads),
		sortLevel(0),
		runningThreads(0),
		fileIndex(0),
		runThread(true),
		isDone(false)
	{
		for(unsigned int i=0; i < num_threads; ++i)
		    workers.push_back(std::thread(&MergeThreadPool::worker_thread,this));

	}

	~MergeThreadPool(){
		for (auto& th : workers){
			if( th.joinable() ) th.join();
		}
	}

	void printLevel(){
		for(unsigned int i = 0; i < numFiles->size(); ++i){
			std::cout << i << " " << numFiles->at(i) << std::endl;
		}
	}

	void printList(){
		for(unsigned int i = 0; i < mergeFiles->size(); ++i){
			std::cout << i << " " << mergeFiles->at(i).file_in_1 << std::endl;
			std::cout << i << " " << mergeFiles->at(i).file_in_2 << std::endl;
			std::cout << i << " " << mergeFiles->at(i).file_out << std::endl;
		}
	}

	void startMerge(){

        //set range of merge level
		unsigned int _from = 0;
		unsigned int _to = numFiles->at(0)/2;

		for(unsigned int i = 1; i < numFiles->size(); ++i){
			std::cout << "_from=" << _from << std::endl;
			std::cout << "_to=" << _to << std::endl;

			unsigned int _level = _to - _from; //control merge level

			std::cout << "_level=" << _level << std::endl;

			for(unsigned int k = _from; k < _to; ++k){

				std::this_thread::sleep_for(std::chrono::microseconds(1000));

				 //--check if threads are available
	        	 if(runningThreads.load(std::memory_order_relaxed) <= num_threads){
	        	    isWorkerAval.notify_one(); //sent signal to a thread that data is ready to be processed
	        	 }else{
	           	   { //Wait until a thread is available
	                   std::unique_lock<std::mutex> lk(Mutex);
	                   isThreadAval.wait(lk);
	           	   }
	        	 }

			}

			//----wait for level to be completed----
             wait_for_level(_level);

             //----------start next level------------
             sortLevel.store(0,std::memory_order_relaxed);
			_from = _to;
			_to  = _from + numFiles->at(i)/2;

			std::cout << "------------------------\n";
		}

		std::cout << "_from=" << _from << std::endl;
		std::cout << "_to=" << _to << std::endl;


		for(unsigned int k = _from; k < _to; ++k){
			std::this_thread::sleep_for(std::chrono::microseconds(1000));
/*
			 //--check if threads are available
        	 if(runningThreads.load(std::memory_order_relaxed) <= num_threads){
        	    isWorkerAval.notify_one(); //sent signal to a thread that data is ready to be processed
        	 }else{
           	   { //Wait until a thread is available
                   std::unique_lock<std::mutex> lk(Mutex);
                   isThreadAval.wait(lk);
           	   }
        	 }
 */
		}

		std::cout << "-------last-------------\n";

		//runThread=false;
		runThread.store(false,std::memory_order_relaxed);
		isWorkerAval.notify_all();
        //isDone.store(true,std::memory_order_relaxed);
	}

};


#endif /* MERGE_THREAD_POOL_HPP_ */
