#ifndef IMERGEHEAP_H_
#define IMERGEHEAP_H_

class AbstractMergeHeap
{
public:
	virtual void meld(AbstractMergeHeap&& heap) = 0;

	void insert(int key)
	{
		AbstractMergeHeap* heap = createHeap(key);
		meld(std::move(*heap));
		delete heap;
	}

	virtual AbstractMergeHeap* createHeap(int key) = 0;

	virtual bool empty() const = 0;

	virtual int getMin() const = 0;

	virtual int extractMin() = 0;

	virtual ~AbstractMergeHeap()
	{
	}
};

#endif
