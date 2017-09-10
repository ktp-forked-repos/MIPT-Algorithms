#include <mutex>
#include <atomic>
#include <condition_variable>
#include <future>
#include <vector>

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