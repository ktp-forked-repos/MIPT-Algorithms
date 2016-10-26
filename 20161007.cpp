#include <iostream>
#include <mutex>
#include <thread>
#include <set>
#include <cassert>

#include "headers/debug.h"
using namespace std;

thread_local static set<int> minId;
class Mutex {
public:
	int id;
	mutex m;
	
	Mutex(int id): id(id) {}
	
	void lock() {
		dbg(minId);
		assert(minId.empty() || *minId.begin() < id);
		m.lock();
		minId.insert(id);
	}
	
	void unlock() {
		m.unlock();
		assert(minId.find(id) != minId.end());
		minId.erase(id);
	}
};

Mutex m1(1), m2(2);
//mutex m1, m2;
void func(int id) {
	if (id == 0) {
		m1.lock();
		this_thread::sleep_for(100ms);
		m2.lock();
		// some work
		m2.unlock();
		m1.unlock();
	} else {
		m2.lock();
		this_thread::sleep_for(100ms);
		m1.lock();
		// some work
		m1.unlock();
		m2.unlock();
	}
}

int main() {
    thread t1(func, 0);
    thread t2(func, 1);
    
    t1.join();
    t2.join();
    return 0;
}
