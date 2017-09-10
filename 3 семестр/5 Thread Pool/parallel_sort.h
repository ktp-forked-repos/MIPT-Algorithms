#include <deque>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <future>
#include <vector>
#include <algorithm>

// Блокирующая очередь (из второй задачи), с измененением функции pop
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

	// возвращает true <=> успешно записал значение в v
	bool pop(Value &v, bool block_if_empty = true) {
		std::unique_lock<std::mutex> lock(mutex_);
		while (container_.size() == 0) {
			if (is_shutdown_) {
				throw std::logic_error("Call pop on empty queue!");
			}
			if (!block_if_empty) {
				return false;
			}
			cv_something_push_.wait(lock);
		}
		v = std::move(container_.front());
		container_.pop_front();
		cv_something_pop_.notify_one();
		return true;
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

// Пул потоков (из первой задачи), с добавлением функции wait
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

	// единственная новая функция
	void wait(std::future<void> future) {
		while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
			queue_type pair;
			bool success = queue_.pop(pair, false);
			if (!success) {
				// В очереди нет задач, значит future, которую мы ждём уже кто-то выполняет
				future.get();
				return;
			}
			pair.second();
		}
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

class parallel_sort {
public:
	parallel_sort(size_t worker_number) : worker_number_(worker_number) {}

	template<typename RandomAccessIterator, typename Compare>
	void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comparator) {
		thread_pool<void> pool(worker_number_);
		std::future<void> main_future = pool.submit(get_lambda(pool, first, 0, last - first, comparator));
		main_future.get();
		pool.shutdown();
	}

private:
	static const size_t MIN_LENGTH = 1000;

	template<typename RandomAccessIterator, typename Compare>
	static std::function<void()> get_lambda(thread_pool<void> &pool, RandomAccessIterator &first0, size_t first, size_t last, Compare comparator) {
		if (last - first <= MIN_LENGTH) {
			return [&first0, first, last, comparator] { std::sort(first0 + first, first0 + last, comparator); };
		} else {
			return [&pool, &first0, first, last, comparator] {
				std::vector<size_t> lasts = {last};
				std::vector<std::future<void>> futures;
				while (lasts.back() - first > MIN_LENGTH) {
					lasts.push_back(first + (lasts.back() - first) / 2);
				}
				for (size_t i = 1; i < lasts.size(); ++i) {
					futures.emplace_back(pool.submit(get_lambda(pool, first0, lasts[i], lasts[i - 1], comparator)));
				}

				std::sort(first0 + first, first0 + lasts.back(), comparator);
				for (int i = (int) lasts.size() - 2; i >= 0; --i) {
					pool.wait(std::move(futures[i]));
					std::inplace_merge(first0 + first, first0 + lasts[i + 1], first0 + lasts[i], comparator);
				}
			};
		}
	}

	size_t worker_number_;
};