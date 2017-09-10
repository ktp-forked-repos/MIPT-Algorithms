#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

class IHeap
{
public:
	virtual void push(int value) = 0;
	virtual int min() const = 0;
	virtual void extractMin() = 0;
	virtual int size() const = 0;
	virtual ~IHeap() {}
};

class MinHeap : public IHeap
{
public:
	virtual void push(int value)
	{
		array_.push_back(value);
		siftUp(array_.size() - 1);
	}
	
	virtual int min() const
	{
		return array_[0];
	}
	
	virtual void extractMin()
	{
		array_[0] = array_.back();
		array_.pop_back();
		siftDown(0);
	}
	
	virtual int size() const
	{
		return array_.size();
	}
private:
	void siftUp(int index)
	{
		int parent;
		while (index > 0 && array_[index] < array_[parent = (index - 1) / 2])
		{
			std::swap(array_[index], array_[parent]);
			index = parent;
		}
	}
	
	void siftDown(size_t index)
	{
		size_t l = index * 2 + 1;
		size_t r = index * 2 + 2;
		size_t iMin = index;
		if (l < array_.size() && array_[l] < array_[iMin])
			iMin = l;
		if (r < array_.size() && array_[r] < array_[iMin])
			iMin = r;
		if (iMin != index)
		{
			std::swap(array_[index], array_[iMin]);
			siftDown(iMin);
		}
	}
	
	std::vector<int> array_;
};

class MinHeapSlow : public IHeap
{
public:
	virtual void push(int value)
	{
		array_.push_back(value);
	}
	
	virtual int min() const
	{
		return *std::min_element(array_.begin(), array_.end());
	}
	
	virtual void extractMin()
	{
		array_.erase(std::find(array_.begin(), array_.end(), min()));
	}
	
	virtual int size() const
	{
		return array_.size();
	}
private:
	std::vector<int> array_;
};

bool areEqual(IHeap *heap1, IHeap *heap2)
{
	srand(time(0));
	for (int i = 0; i < 1000; ++i)
		if (heap1->size() == 0 || rand() % 3 == 0)
		{
			int value = rand();
			heap1->push(value);
			heap2->push(value);
		}
		else if (rand() % 2 == 0)
		{
			int min1 = heap1->min();
			int min2 = heap2->min();
			if (min1 != min2)
				return 0;
		}
		else
		{
			heap1->extractMin();
			heap2->extractMin();
		}
	return 1;
}

void heapSort(int *start, int *finish, IHeap *heap)
{
	for (int *it = start; it != finish; ++it)
		heap->push(*it);
	for (int *it = start; it != finish; ++it)
	{
		*it = heap->min();
		heap->extractMin();
	}
}

int main()
{
	IHeap *minHeap = new MinHeap();
	IHeap *minHeapSlow = new MinHeapSlow();
	std::cout << areEqual(minHeap, minHeapSlow) << std::endl;
	delete minHeap;
	delete minHeapSlow;
	
	srand(time(0));
	int n = 20;
	int a[n];
	for (int i = 0; i < n; ++i)
		a[i] = rand() % 20;
	
	IHeap *heap = new MinHeap();
	heapSort(a, a + n, heap);
	delete heap;
	
	for (int i = 0; i < n; ++i)
		std::cout << a[i] << " \n"[i == n - 1];
	return 0;
}