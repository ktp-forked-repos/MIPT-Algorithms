#ifndef PRIORITYQUEUEHEAP_H_
#define PRIORITYQUEUEHEAP_H_

#include "AbstractMergeHeap.h"

class PriorityQueue: public AbstractMergeHeap
{
private:
	std::priority_queue<int, std::vector<int>, std::greater<int>> priority_queue;

public:
	explicit PriorityQueue(int key)
	{
		priority_queue.push(key);
	}

	virtual AbstractMergeHeap* createHeap(int key)
	{
		return new PriorityQueue(key);
	}

	virtual void meld(AbstractMergeHeap&& heapBase)
	{
		PriorityQueue && heap = dynamic_cast<PriorityQueue&&>(heapBase);
		while (!heap.priority_queue.empty())
		{
			priority_queue.push(heap.priority_queue.top());
			heap.priority_queue.pop();
		}
	}

	virtual bool empty() const
	{
		return priority_queue.empty();
	}

	virtual int getMin() const
	{
		return priority_queue.top();
	}

	virtual int extractMin()
	{
		int min = priority_queue.top();
		priority_queue.pop();
		return min;
	}

	virtual ~PriorityQueue()
	{
	}
};

#endif
