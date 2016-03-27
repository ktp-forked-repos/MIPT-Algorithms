#ifndef BINOMIALHEAP_H_
#define BINOMIALHEAP_H_

#include "AbstractMergeHeap.h"

struct BinomialHeapNode
{
	int key;
	std::vector<BinomialHeapNode*> children;

	BinomialHeapNode(int key, std::vector<BinomialHeapNode*> children)
			: key(key), children(children)
	{
	}

	explicit BinomialHeapNode(int key)
			: key(key)
	{
	}

	virtual ~BinomialHeapNode()
	{
		for (BinomialHeapNode* node : children)
			delete node;
	}

	static std::vector<BinomialHeapNode*> copyVectorNodes(const std::vector<BinomialHeapNode*> &vector)
	{
		std::vector<BinomialHeapNode*> copy;
		for (size_t i = 0; i < vector.size(); ++i)
			copy.push_back(vector[i] == nullptr ? nullptr : new BinomialHeapNode(vector[i]->key, copyVectorNodes(vector[i]->children)));
		return copy;
	}

};

class BinomialHeap: public AbstractMergeHeap
{
private:
	std::vector<BinomialHeapNode*> nodes_;

	BinomialHeapNode* mergeNodes(BinomialHeapNode* node1, BinomialHeapNode* node2)
	{
		assert(node1 != nullptr && node2 != nullptr);
		if (node1->key > node2->key)
			std::swap(node1, node2);
		node1->children.push_back(node2);
		return node1;
	}

	std::vector<BinomialHeapNode*> mergeListsNodes(std::vector<BinomialHeapNode*> list1, std::vector<BinomialHeapNode*> list2)
	{
		if (list1.size() < list2.size())
			list1.swap(list2);

		BinomialHeapNode* nodeCarry = nullptr;
		for (size_t i = 0; i < list1.size(); ++i)
		{
			std::vector<BinomialHeapNode*> nodesToMerge;
			if (list1[i] != nullptr)
				nodesToMerge.push_back(list1[i]);
			if (i < list2.size() && list2[i] != nullptr)
				nodesToMerge.push_back(list2[i]);
			if (nodeCarry != nullptr)
				nodesToMerge.push_back(nodeCarry);
			nodeCarry = (nodesToMerge.size() >= 2) ? mergeNodes(nodesToMerge[0], nodesToMerge[1]) : nullptr;
			list1[i] = (nodesToMerge.size() % 2 == 1) ? nodesToMerge.back() : nullptr;
		}
		if (nodeCarry != nullptr)
			list1.push_back(nodeCarry);
		return list1;
	}

public:
	BinomialHeap()
	{
	}

	explicit BinomialHeap(int key)
	{
		nodes_.push_back(new BinomialHeapNode(key));
	}

	virtual AbstractMergeHeap* createHeap(int key)
	{
		return new BinomialHeap(key);
	}

	BinomialHeap(const BinomialHeap& heap)
			: nodes_(BinomialHeapNode::copyVectorNodes(heap.nodes_))
	{
	}

	BinomialHeap& operator =(const BinomialHeap& node)
	{
		for (BinomialHeapNode* node : nodes_)
			delete node;
		nodes_ = BinomialHeapNode::copyVectorNodes(node.nodes_);
		return *this;
	}

	BinomialHeap(BinomialHeap&& heap)
			: nodes_(std::move(heap.nodes_))
	{
	}

	virtual void meld(AbstractMergeHeap&& heapBase)
	{
		BinomialHeap&& heap = dynamic_cast<BinomialHeap&&>(heapBase);
		nodes_ = mergeListsNodes(nodes_, heap.nodes_);
		heap.nodes_.clear();
	}

	virtual bool empty() const
	{
		for (BinomialHeapNode* node : nodes_)
			if (node != nullptr)
				return 0;
		return 1;
	}

	virtual int getMin() const
	{
		int min = INT_MAX;
		for (BinomialHeapNode* node : nodes_)
			if (node != nullptr)
				min = std::min(min, node->key);
		return min;
	}

	virtual int extractMin()
	{
		int min = getMin();
		for (size_t i = 0; i < nodes_.size(); ++i)
			if (nodes_[i] != nullptr && nodes_[i]->key == min)
			{
				std::vector<BinomialHeapNode*> children = nodes_[i]->children;
				nodes_[i]->children.clear();
				delete nodes_[i];
				nodes_[i] = nullptr;
				nodes_ = mergeListsNodes(nodes_, children);
				break;
			}
		return min;
	}

	virtual ~BinomialHeap()
	{
		for (BinomialHeapNode* node : nodes_)
			delete node;
	}

};

#endif
