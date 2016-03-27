#ifndef MYDEQUE_H_
#define MYDEQUE_H_

#include "MyIterator.h"

template<typename T>
class MyDeque
{
public:
	typedef MyIterator<T, T*, T&> iterator;
	typedef MyIterator<T, const T*, const T&> const_iterator;
	typedef std::reverse_iterator<MyIterator<T, T*, T&>> reverse_iterator;
	typedef std::reverse_iterator<MyIterator<T, const T*, const T&>> const_reverse_iterator;

	explicit MyDeque(size_t capacity = INITIAL_CAPACITY)
			: buffer_(new T[capacity]), indexFirst_(0), size_(0), capacity_(capacity)
	{
	}

	MyDeque(const MyDeque& other)
			: buffer_(new T[other.capacity_]), indexFirst_(0), size_(other.size_), capacity_(other.capacity_)
	{
		std::copy(other.begin(), other.end(), buffer_);
	}

	MyDeque(MyDeque&& other)
			: size_(other.size_)
					, capacity_(other.capacity_)
					, indexFirst_(other.indexFirst_)
					, buffer_(other.buffer_)
	{
		other.buffer_ = nullptr;
	}

	MyDeque& operator =(const MyDeque& other)
	{
		size_ = other.size_;
		if (capacity_ <= size_)
		{
			capacity_ = other.capacity_;
			delete[] buffer_;
			buffer_ = new T[capacity_];
		}
		std::copy(other.begin(), other.end(), buffer_);
		return *this;
	}

	void push_back(T object)
	{
		tryIncreaseBuffer();
		buffer_[(indexFirst_ + size_) % capacity_] = object;
		++size_;
	}

	void push_front(T object)
	{
		tryIncreaseBuffer();
		indexFirst_ = (capacity_ + indexFirst_ - 1) % capacity_;
		buffer_[indexFirst_] = object;
		++size_;
	}

	void pop_back()
	{
		tryDecreaseBuffer();
		--size_;
	}

	void pop_front()
	{
		tryDecreaseBuffer();
		--size_;
		indexFirst_ = (indexFirst_ + 1) % capacity_;
	}

	T& operator [](size_t index)
	{
		return buffer_[(indexFirst_ + index) % capacity_];
	}

	const T& operator [](size_t index) const
			{
		return buffer_[(indexFirst_ + index) % capacity_];
	}

	T& back()
	{
		return buffer_[(indexFirst_ + size_ - 1) % capacity_];
	}

	T& front()
	{
		return buffer_[indexFirst_];
	}

	const T& back() const
	{
		return buffer_[(indexFirst_ + size_ - 1) % capacity_];
	}

	const T& front() const
	{
		return buffer_[indexFirst_];
	}

	bool empty() const
	{
		return size_ == 0;
	}

	size_t size() const
	{
		return size_;
	}

	~MyDeque()
	{
		delete[] buffer_;
	}

	iterator begin()
	{
		return iterator(buffer_, buffer_ + capacity_, buffer_ + indexFirst_, &indexFirst_);
	}

	const_iterator begin() const
	{
		return const_iterator(buffer_, buffer_ + capacity_, buffer_ + indexFirst_, &indexFirst_);
	}

	const_iterator cbegin() const
	{
		return const_iterator(buffer_, buffer_ + capacity_, buffer_ + indexFirst_, &indexFirst_);
	}

	iterator end()
	{
		return iterator(buffer_, buffer_ + capacity_, buffer_ + (indexFirst_ + size_) % capacity_, &indexFirst_);
	}

	const_iterator end() const
	{
		return const_iterator(buffer_, buffer_ + capacity_, buffer_ + (indexFirst_ + size_) % capacity_, &indexFirst_);
	}

	const_iterator cend() const
	{
		return const_iterator(buffer_, buffer_ + capacity_, buffer_ + (indexFirst_ + size_) % capacity_, &indexFirst_);
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(cend());
	}

	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator(cend());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(cbegin());
	}

	const_reverse_iterator crend() const
	{
		return const_reverse_iterator(cbegin());
	}

private:
	T *buffer_;
	size_t indexFirst_;
	size_t size_;
	size_t capacity_;

	static const size_t INITIAL_CAPACITY = 4;

	void replaceBuffer(size_t newCapacity)
	{
		T *newBuffer = new T[newCapacity];
		std::copy(begin(), end(), newBuffer);
		delete[] buffer_;
		buffer_ = newBuffer;
		indexFirst_ = 0;
		capacity_ = newCapacity;
	}

	void tryIncreaseBuffer()
	{
		if (size_ + 1 == capacity_)
			replaceBuffer(capacity_ * 2);
	}

	void tryDecreaseBuffer()
	{
		if (capacity_ > INITIAL_CAPACITY && capacity_ > size_ * 4)
			replaceBuffer(capacity_ / 2);
	}

};

#endif
