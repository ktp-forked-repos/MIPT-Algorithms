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
		for (size_t i = actual_number_threads_ + thread_index - 1; i > 0; i = (i - 1) / 2)
			mutexes_[(i - 1) / 2].lock(i % 2);
	}
	
	void unlock(size_t thread_index)
	{
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
