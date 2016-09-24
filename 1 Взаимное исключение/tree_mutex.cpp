#include <bits/stdc++.h>
//#include <debug.h>

class peterson_mutex
{
private:
	std::atomic_bool want_[2];
	std::atomic_bool victim_;
public:
	peterson_mutex()
	{
		want_[0] = false;
		want_[1] = false;
		victim_ = 0;
	}
	
	void lock(bool thread_index)
	{
		want_[thread_index] = true;
		victim_ = thread_index;
		while (want_[1 - thread_index] && victim_ == thread_index)
			std::this_thread::yield();
	}
	
	void unlock(bool thread_index)
	{
		want_[thread_index] = false;
	}
};

class tree_mutex
{
private:
	int x;
	size_t actual_number_threads_;
	peterson_mutex *mutexes_;

public:	
	std::vector<size_t> get_indexes(size_t thread_index)
	{
		std::vector<size_t> indexes;
		for (size_t i = actual_number_threads_ + thread_index - 1; i > 0; i = (i - 1) / 2)
			indexes.push_back(i);
		return indexes;
	}
	
	tree_mutex(size_t number_threads)
	{
		actual_number_threads_= 1;
		while (actual_number_threads_ < number_threads)
			actual_number_threads_ *= 2;
		mutexes_ = new peterson_mutex[actual_number_threads_ - 1];
	}
	
	void lock(size_t thread_index)
	{
		//dbgl("lock", thread_index);
		for (size_t i = actual_number_threads_ + thread_index - 1; i > 0; i = (i - 1) / 2)
			mutexes_[(i - 1) / 2].lock(i % 2);
		//dbgl("\tlock", thread_index);
	}
	
	void unlock(size_t thread_index)
	{
		//dbgl("unlock", thread_index);
		std::vector<size_t> indexes = get_indexes(thread_index);
		for (int i = indexes.size() - 1; i >= 0; --i)
			mutexes_[(indexes[i] - 1) / 2].unlock(indexes[i] % 2);
	}
	
	tree_mutex operator=(tree_mutex &other) = delete;
	tree_mutex(tree_mutex &other) = delete;
	
	~tree_mutex()
	{
		delete[] mutexes_;
	}
};

int i;
std::atomic_int ai;
std::atomic_int ai2;

std::mutex m2;
void func(size_t thread_index, tree_mutex *m)
{
	size_t n = 10000;
	//dbg(thread_index, n);
	while (n--)
	{
		m->lock(thread_index);
		assert(m2.try_lock());
		++i;
		++ai;
		++ai2;
		m2.unlock();
		m->unlock(thread_index);
	}
}

void test()
{
	srand(time(0));
	i = 0;
	ai = 0;
	ai2 = 0;
	
	size_t number_threads = 3;
	//dbg(number_threads);
	std::vector<std::thread> threads;
	tree_mutex m(number_threads);
	for (size_t thread_index = 0; thread_index < number_threads; ++thread_index)
		threads.emplace_back(func, thread_index, &m);
	for (size_t thread_index = 0; thread_index < number_threads; ++thread_index)
		threads[thread_index].join();
	//dbg(i, ai, ai2);
	assert(ai == ai2);
	assert(i == ai);
	
	exit(0);
}

int main()
{
	test();
	return 0;
}
