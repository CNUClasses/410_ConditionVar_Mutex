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

//there is a race condition
//where waiter can miss being notified,
//or a spurious wakeup will cause incorrect behavior
void wait() {
	std::thread::id this_id = std::this_thread::get_id();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));	//wait before aquiring lock below
	
	unique_lock<mutex> lck(m);
	cout <<"Thread "<<this_id<< ": About to wait"<< endl; //makes it hard to see thread switches

	//uh ohh no check for condition
	cv.wait(lck);

	cout <<"Thread "<<this_id<< ": Finished waiting, exiting thread"<< endl; //makes it hard to see thread switches
}

void notify() {

	std::thread::id this_id = std::this_thread::get_id();
	{
		unique_lock<mutex> lck(m);
		cout <<"Thread "<<this_id<< ": About to notify"<< endl; //makes it hard to see thread switches
	}	

	cv.notify_all();
	
	cout <<"Thread "<<this_id<< ": Notified, exiting thread"<< endl; //makes it hard to see thread switches
}

////correct solution
//void wait() {
//	std::thread::id this_id = std::this_thread::get_id();
//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));	//wait before aquiring lock below
//
//	unique_lock<mutex> lck(m);
//	cout <<"Thread "<<this_id<< ": About to wait"<< endl; //makes it hard to see thread switches
//
//	while (!ready){
//		cout <<"Thread "<<this_id<< ": about to call cv.wait()"<< endl;
//		cv.wait(lck);
//	}
//
//	cout <<"Thread "<<this_id<< ": Finished waiting, exiting thread"<< endl; //makes it hard to see thread switches
//}
//
//void notify() {
//	std::thread::id this_id = std::this_thread::get_id();
//	{
//		unique_lock<mutex> lck(m);
//		cout <<"Thread "<<this_id<< ": About to notify"<< endl; //makes it hard to see thread switches
//		ready = true;
//	}
//	cv.notify_all();
//
//	cout <<"Thread "<<this_id<< ": Notified, exiting thread"<< endl; //makes it hard to see thread switches
//}
int main(){
	thread t_w(wait);
	thread t_n(notify);	
	
	t_w.join();
	t_n.join();
	cout << "Done and exiting program"<< endl;
}

