#include <iostream>
#include <mutex>
#include <condition_variable>
#include <atomic>

class barrier {
public:
	explicit barrier(size_t num_threads) : num_threads_(num_threads), epoch_(0), counter_(0) {}

	void enter() {
		std::unique_lock<std::mutex> lock(mutex_);
		size_t current_epoch = epoch_;
		++counter_;
		if (counter_ == num_threads_) {
			counter_ = 0;
			++epoch_;
			condition_variable_.notify_all();
		} else {
			do {
				condition_variable_.wait(lock);
			} while (current_epoch == epoch_); // loop for spurious wakeup
		}
	}

private:
	const size_t num_threads_;

	std::atomic_size_t epoch_;
	std::atomic_size_t counter_;
	std::mutex mutex_;
	std::condition_variable condition_variable_;
};