#include<thread>
#include<string>
#include<iostream>
#include<vector>
#include<functional>
#include"sfuns.h"


int main(int argc, char** argv){

    int nthreads = 1, nterms = (int)1e8; /* default values */
    // int64_t nterms = (int64_t)1e8;
    for(int i = 0; i < argc; i++) {
    std::string arg = argv[i];
    if(arg=="--threads" && i+1<argc) nthreads=std::stoi(argv[i+1]);
    if(arg=="--terms" && i+1<argc) nterms=(int)std::stod(argv[i+1]);
    }
    std::cout << "terms: "<<nterms<<"\n";
	std::cout << "threads: "<<nthreads<<"\n";

    std::vector<std::thread> threads;
    threads.reserve(nthreads); // reserve empty slots for our threads


	std::vector< sfuns::data > data(nthreads);
	for(int i=0;i<nthreads;++i){
		data[i].start = 1 + (nterms/nthreads) * i;
		data[i].end = 1 + (nterms/nthreads) * (i+1);
		threads.emplace_back(sfuns::harmonic,std::ref(data[i]));        //emplace back basically append
	}
	for (std::thread &thread : threads)thread.join();
	double total=0;
	for (sfuns::data &d : data)total += d.sum;
	std::cout<<"total sum= "<<total<<"\n";

   return 0;
}