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

//if following defined then there is a race condition
//where waiter can miss being notified, 
//or a spurious wakeup will cause incorrect behavior
#define INCORRECT_NO_SHARED_VAR

void wait() {
	std::thread::id this_id = std::this_thread::get_id();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));	//wait before aquiring lock below
	
	unique_lock<mutex> lck(m);
	cout <<"Thread "<<this_id<< ": About to wait"<< endl; //makes it hard to see thread switches
#ifdef INCORRECT_NO_SHARED_VAR


	cv.wait(lck);
#else
	while (!ready){
		cout <<"Thread "<<this_id<< ": about to call cv.wait()"<< endl;
		cv.wait(lck);
	}
#endif

	cout <<"Thread "<<this_id<< ": Finished waiting, exiting thread"<< endl; //makes it hard to see thread switches
}

void notify() {

	std::thread::id this_id = std::this_thread::get_id();
#ifdef INCORRECT_NO_SHARED_VAR
	{
		unique_lock<mutex> lck(m);
		cout <<"Thread "<<this_id<< ": About to notify"<< endl; //makes it hard to see thread switches
	}	
#else
	{
		unique_lock<mutex> lck(m);
		cout <<"Thread "<<this_id<< ": About to notify"<< endl; //makes it hard to see thread switches
		ready = true;
	}
#endif	
	cv.notify_all();
	
	cout <<"Thread "<<this_id<< ": Notified, exiting thread"<< endl; //makes it hard to see thread switches
}


int main(){
	thread t_w(wait);
	thread t_n(notify);	
	
	t_w.join();
	t_n.join();
	cout << "Done and exiting program"<< endl;
}

