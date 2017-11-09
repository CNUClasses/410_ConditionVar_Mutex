//============================================================================
// Name        : 410_ConditionVar_Mutex.cpp
// Author      : 
// Version     :
// Copyright   : Steal this code!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>

using namespace std;

//globals
mutex 				m;	
condition_variable 	cv;
bool 				ready = false;	//the condition

void notify() {
	std::thread::id this_id = std::this_thread::get_id();
	
	unique_lock<mutex> lck(m);
	cout <<"Thread "<<this_id<< ": About to notify"<< endl; //makes it hard to see thread switches

	ready = true;
	cv.notify_all();
	
	cout <<"Thread "<<this_id<< ": Notified, exiting thread"<< endl; //makes it hard to see thread switches
}

void wait() {
	std::thread::id this_id = std::this_thread::get_id();
	
	unique_lock<mutex> lck(m);
	cout <<"Thread "<<this_id<< ": About to wait"<< endl; //makes it hard to see thread switches
	while (!ready){
		cv.wait(lck);
	}
	ready = false;				//1 comment this out spurious wakeups kill it all
	cout <<"Thread "<<this_id<< ": Finished waiting, exiting thread"<< endl; //makes it hard to see thread switches

}
int main(){
	thread t_w(wait);
	thread t_n(notify);	
	
	t_w.join();
	t_n.join();
	cout << "Done and exiting program"<< endl;
}

