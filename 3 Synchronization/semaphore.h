#include <mutex>
#include <iostream>
#include <thread>
#include <condition_variable>

class Semaphore {
public:
	Semaphore()
			: counter_(0) {}

	void post() {
		std::unique_lock<std::mutex> lock(m_);
		++counter_;
		cv_.notify_one();
	}

	void wait() {
		std::unique_lock<std::mutex> lock(m_);
		cv_.wait(lock, [&] { return counter_ > 0; });
		--counter_;
	}

private:
	std::mutex m_;
	std::condition_variable cv_;
	int counter_;
};

class blocking_flag {
public:
	blocking_flag() : ready_(false) {}

	void wait() {
		std::unique_lock<std::mutex> lock(mtx_);
		while (!ready_.load()) {
			ready_cond_.wait(lock);
		}
	}

	void set() {
		ready_.store(true);
		ready_cond_.notify_all();
	}

private:
	std::atomic<bool> ready_;
	std::mutex mtx_;
	std::condition_variable ready_cond_;
};