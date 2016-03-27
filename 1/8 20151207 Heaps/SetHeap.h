#ifndef SETHEAP_H_
#define SETHEAP_H_

#include "AbstractMergeHeap.h"

class SetHeap: public AbstractMergeHeap
{
private:
	std::multiset<int> set;

public:
	SetHeap(int key)
	{
		set.insert(key);
	}

	virtual AbstractMergeHeap* createHeap(int key)
	{
		return new SetHeap(key);
	}

	virtual void meld(AbstractMergeHeap&& heapBase)
	{
		SetHeap&& heap = dynamic_cast<SetHeap&&>(heapBase);
		set.insert(heap.set.begin(), heap.set.end());
		heap.set.clear();
	}

	virtual bool empty() const
	{
		return set.empty();
	}

	virtual int getMin() const
	{
		return *set.begin();
	}

	virtual int extractMin()
	{
		int min = *set.begin();
		set.erase(set.begin());
		return min;
	}

	virtual ~SetHeap()
	{
	}
};

#endif
