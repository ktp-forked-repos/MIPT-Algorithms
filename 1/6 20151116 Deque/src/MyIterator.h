#ifndef MYITERATOR_H_
#define MYITERATOR_H_

template<typename T, typename Pointer, typename Reference>
class MyIterator: public std::iterator<std::random_access_iterator_tag, T>
{
public:
	MyIterator()
			: begin_(), end_(), current_(), indexFirst_()
	{
	}

	MyIterator(T* begin, T* end, Pointer current, const size_t *iFirst)
			: begin_(begin), end_(end), current_(current), indexFirst_(iFirst)
	{
	}

	bool operator ==(const MyIterator &other) const
			{
		return current_ == other.current_;
	}

	bool operator !=(const MyIterator &other) const
			{
		return current_ != other.current_;
	}

	Reference operator *() const
	{
		return *current_;
	}

	Pointer operator ->() const
	{
		return current_;
	}

	MyIterator& operator ++()
	{
		++current_;
		if (current_ == end_)
			current_ = begin_;
		return *this;
	}

	MyIterator operator ++(int)
	{
		MyIterator temp = *this;
		++(*this);
		return temp;
	}

	MyIterator& operator --()
	{
		if (current_ == begin_)
			current_ = end_;
		--current_;
		return *this;
	}

	MyIterator operator --(int)
	{
		MyIterator temp = *this;
		--(*this);
		return temp;
	}

	MyIterator& operator +=(int n)
	{
		size_t capacity = end_ - begin_;
		current_ = begin_ + (capacity + *indexFirst_ + getDequeIndex() + n) % capacity;
		return *this;
	}

	MyIterator operator +(int n) const
			{
		MyIterator temp = *this;
		return temp += n;
	}

	MyIterator& operator -=(int n)
	{
		return *this += -n;
	}

	MyIterator operator -(int n) const
			{
		return *this + -n;
	}

	int operator -(const MyIterator& other) const
			{
		return static_cast<int>(getDequeIndex()) - static_cast<int>(other.getDequeIndex());
	}

	Reference operator [](int n) const
			{
		return *(*this + n);
	}

	bool operator <(const MyIterator &other) const
			{
		return getDequeIndex() < other.getDequeIndex();
	}

	bool operator >(const MyIterator &other) const
			{
		return getDequeIndex() > other.getDequeIndex();
	}

	bool operator <=(const MyIterator &other) const
			{
		return getDequeIndex() <= other.getDequeIndex();
	}

	bool operator >=(const MyIterator &other) const
			{
		return getDequeIndex() >= other.getDequeIndex();
	}

	size_t getDequeIndex() const
	{
		size_t bufferIndex = current_ - begin_;
		int dequeIndex = bufferIndex - *indexFirst_;
		if (dequeIndex < 0)
			dequeIndex += end_ - begin_;
		return static_cast<size_t>(dequeIndex);
	}

private:
	T* begin_;
	T* end_;
	Pointer current_;
	const size_t *indexFirst_;
};

template<typename T, typename Pointer, typename Reference>
MyIterator<T, Pointer, Reference> operator +(int n, const MyIterator<T, Pointer, Reference> &iterator)
{
	return iterator + n;
}

template<typename Stream, typename T, typename Pointer, typename Reference>
Stream& operator <<(Stream &stream, const MyIterator<T, Pointer, Reference> &iterator)
{
	return stream << "MyIterator (index=" << iterator.getDequeIndex() << ", value=" << *iterator << ")";
}

#endif
