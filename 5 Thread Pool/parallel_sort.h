#include "/home/dima/C++/debug.h"

#include <mutex>
#include <atomic>
#include <condition_variable>
#include <future>
#include <vector>
#include <algorithm>
#include <deque>

// Блокирующая очередь (из второй задачи)
template<class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:
	explicit thread_safe_queue(size_t capacity) : capacity_(capacity), is_shutdown_(false) {}

	void enqueue(Value v) {
		std::unique_lock<std::mutex> lock(mutex_);
		while (is_shutdown_ || container_.size() == capacity_) {
			if (is_shutdown_) {
				throw std::logic_error("Call enqueue after shutdown!");
			}
			cv_something_pop_.wait(lock);
		}
		container_.push_back(std::move(v));
		cv_something_push_.notify_one();
	}

	void pop(Value &v) {
		std::unique_lock<std::mutex> lock(mutex_);
		while (container_.size() == 0) {
			if (is_shutdown_) {
				throw std::logic_error("Call pop on empty queue!");
			}
			cv_something_push_.wait(lock);
		}
		v = std::move(container_.front());
		container_.pop_front();
		cv_something_pop_.notify_one();
	}

	void shutdown() {
		is_shutdown_ = true;
		cv_something_push_.notify_all();
		cv_something_pop_.notify_all();
	}

private:
	size_t capacity_;
	Container container_;
	std::atomic_bool is_shutdown_;
	std::mutex mutex_;
	std::condition_variable cv_something_push_;
	std::condition_variable cv_something_pop_;
};

// Пул потокв (из первой задачи)
template<class Value>
class thread_pool {
public:
	explicit thread_pool(size_t workers_number) : workers_number_(workers_number), queue_(SIZE_MAX), is_shutdown_(false) {
		for (size_t thread_index = 0; thread_index < workers_number; ++thread_index) {
			threads_.emplace_back(thread_function, &queue_);
		}
	}

	std::future<Value> submit(std::function<Value()> function) {
		if (is_shutdown_) {
			throw std::logic_error("Call submit after shutdown!");
		}
		std::packaged_task<Value()> task(function);
		std::future<Value> future = task.get_future();
		queue_.enqueue({false, std::move(task)});
		return future;
	}

	void shutdown() {
		is_shutdown_ = true;
		for (size_t thread_index = 0; thread_index < workers_number_; ++thread_index) {
			queue_.enqueue({true, std::packaged_task<Value()>()});
		}
		for (std::thread &thread : threads_) {
			thread.join();
		}
	}

	~thread_pool() noexcept(false) {
		if (!is_shutdown_) {
			throw std::logic_error("You forgot call shutdown!");
		}
	}

private:
	typedef std::pair<bool, std::packaged_task<Value()>> queue_type;

	static void thread_function(thread_safe_queue<queue_type> *queue) {
		while (true) {
			queue_type pair;
			queue->pop(pair);
			if (pair.first) {
				break;
			}
			pair.second();
		}
	}

	size_t workers_number_;
	std::vector<std::thread> threads_;
	thread_safe_queue<queue_type> queue_;
	std::atomic_bool is_shutdown_;
};

/*template<typename RandomAccessIterator, typename Compare>
class Sort {
public:
	Sort(thread_pool<void> &pool, RandomAccessIterator first, RandomAccessIterator last, Compare comparator) : pool(pool), first(first), last(last), comparator(comparator) {}

	void operator()() {
		size_t length = last - first;
		if (length <= 10) {
			std::sort(first, last, comparator);
		} else {
			RandomAccessIterator middle = first + length / 2;
			pool.submit(Sort<RandomAccessIterator, Compare>(pool, first, middle, comparator));
			pool.submit(Sort<RandomAccessIterator, Compare>(pool, middle, last, comparator));

			RandomAccessIterator first_for_lambda = first;
			RandomAccessIterator last_for_lambda = last;
			pool.submit([length, first_for_lambda, middle, last_for_lambda] {
				std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> buffer(length);
				std::merge(first_for_lambda, middle, middle, last_for_lambda, buffer.begin());
				std::copy(buffer.begin(), buffer.end(), first_for_lambda);
			});
		}
	}

private:
	thread_pool<void> &pool;
	RandomAccessIterator first;
	RandomAccessIterator last;
	Compare comparator;
};*/

class parallel_sort {
public:
	parallel_sort(size_t worker_number) : worker_number_(worker_number) {}

	template<typename RandomAccessIterator, typename Compare>
	void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comparator) {
		thread_pool<void> pool(worker_number_);
		recursion(pool, first, last, comparator);
//		bfs(pool, first, last, comparator);
		pool.shutdown();
	}

private:
	static const size_t MIN_LENGTH = 100;

//	template<typename RandomAccessIterator, typename Compare>
//	void bfs(thread_pool<void> &pool, RandomAccessIterator first, RandomAccessIterator last, Compare comparator) {
//		size_t length = last - first;
//		if (length <= 100) {
//			pool.submit([first, last, comparator] { std::sort(first, last, comparator); });
//		} else {
//			RandomAccessIterator middle = first + length / 2;
//			recursion(pool, first, middle, comparator);
//			recursion(pool, middle, last, comparator);
//			pool.submit([middle, first, last, comparator, length] {
//				std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> buffer(length);
//				std::merge(first, middle, middle, last, buffer.begin());
//				std::copy(buffer.begin(), buffer.end(), first);
//			});
//		}
//	}

	template<typename RandomAccessIterator, typename Compare>
	std::future<void> recursion(thread_pool<void> &pool, RandomAccessIterator first, RandomAccessIterator last, Compare comparator) {
		size_t length = last - first;
		if (length <= 100) {
			return pool.submit([first, last, comparator] { std::sort(first, last, comparator); });
		} else {
			RandomAccessIterator middle = first + length / 2;
			std::future<void> left = recursion(pool, first, middle, comparator);
			std::future<void> right = recursion(pool, middle, last, comparator);
			// , left = std::move(left), right = std::move(right)
			auto x = [middle, first, last, comparator, length, right = std::move(right)]() mutable {
//				left.get();
//				right.get();
				std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> buffer(length);
				std::merge(first, middle, middle, last, buffer.begin());
				std::copy(buffer.begin(), buffer.end(), first);
			};
			pool.submit(x);
//			std::future<void> result = pool.submit(x);
			return left;
		}
	}

	size_t worker_number_;
};

//*
#include <cstdlib>
//#include <stdlib.h>
#include <cassert>
using namespace std;

int main() {
	parallel_sort sorter(4);
	vector<int> a;
	for (int i = 0; i < 100; ++i) {
		a.push_back(rand() % 10);
	}
	sorter.sort(a.begin(), a.end(), less<int>());
	dbg(a);
	for (int i = 1; i < a.size(); ++i) {
		assert(a[i - 1] <= a[i]);
	}
	return 0;
}//*/
