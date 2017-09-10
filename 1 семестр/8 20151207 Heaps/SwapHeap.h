#ifndef SWAPHEAP_H_
#define SWAPHEAP_H_

#include "AbstractMergeHeap.h"

struct SwapHeapNode
{
	SwapHeapNode* left;
	SwapHeapNode* right;
	int key;

	explicit SwapHeapNode(int key)
			: left(nullptr), right(nullptr), key(key)
	{
	}

	SwapHeapNode(int key, SwapHeapNode* left, SwapHeapNode* right)
			: left(left), right(right), key(key)
	{
	}

	virtual ~SwapHeapNode()
	{
		delete left;
		delete right;
	}

	virtual void update() = 0;
	virtual bool needSwap() = 0;
};

struct LeftHeapNode: public SwapHeapNode
{
	size_t subTreeSize;

	explicit LeftHeapNode(int key)
			: SwapHeapNode(key), subTreeSize(1)
	{
	}

	LeftHeapNode(const LeftHeapNode& node)
			: SwapHeapNode(node.key, copyNode(dynamic_cast<LeftHeapNode*>(node.left)),
					copyNode(dynamic_cast<LeftHeapNode*>(node.right))), subTreeSize(node.subTreeSize)
	{
	}

	virtual ~LeftHeapNode()
	{
	}

	static LeftHeapNode* copyNode(LeftHeapNode* node)
	{
		return node == nullptr ? nullptr : new LeftHeapNode(*node);
	}

	static size_t getSubTreeSize(LeftHeapNode* node)
	{
		return node == nullptr ? 0 : node->subTreeSize;
	}

	virtual void update()
	{
		subTreeSize = getSubTreeSize(dynamic_cast<LeftHeapNode*>(left)) + 1 + getSubTreeSize(dynamic_cast<LeftHeapNode*>(right));
	}

	virtual bool needSwap()
	{
		return LeftHeapNode::getSubTreeSize(dynamic_cast<LeftHeapNode*>(left)) < LeftHeapNode::getSubTreeSize(dynamic_cast<LeftHeapNode*>(right));
	}
};

struct SkewHeapNode: public SwapHeapNode
{
	explicit SkewHeapNode(int key)
			: SwapHeapNode(key)
	{
	}

	SkewHeapNode(int key, SkewHeapNode* left, SkewHeapNode* right)
			: SwapHeapNode(key, left, right)
	{
	}

	SkewHeapNode(const SkewHeapNode& node)
			: SwapHeapNode(key, left, right)
	{
	}

	virtual ~SkewHeapNode()
	{
	}

	static SkewHeapNode* copyNode(SkewHeapNode* node)
	{
		return node == nullptr ? nullptr : new SkewHeapNode(*node);
	}

	virtual void update()
	{
	}

	virtual bool needSwap()
	{
		return 1;
	}

};

template<class Node>
class SwapHeap: public AbstractMergeHeap
{
private:
	Node* root_;

	Node* mergeNodes(Node* node1, Node* node2)
	{
		if (node1 == nullptr || node2 == nullptr)
			return node1 == nullptr ? node2 : node1;
		if (node1->key > node2->key)
			std::swap(node1, node2);
		node1->right = mergeNodes(dynamic_cast<Node*>(node1->right), node2);
		node1->update();
		if (node1->needSwap())
			std::swap(node1->left, node1->right);
		return node1;
	}

public:
	SwapHeap()
			: root_(nullptr)
	{
	}

	explicit SwapHeap(int key)
			: root_(new Node(key))
	{
	}

	virtual AbstractMergeHeap* createHeap(int key)
	{
		return new SwapHeap(key);
	}

	SwapHeap(const SwapHeap& heap)
			: root_(Node::copyNode(heap.root_))
	{
	}

	SwapHeap& operator =(const SwapHeap& heap)
	{
		delete root_;
		root_ = Node::copyNode(heap.root_);
		return *this;
	}

	SwapHeap(SwapHeap&& heap)
			: root_(heap.root_)
	{
		heap.root_ = nullptr;
	}

	virtual void meld(AbstractMergeHeap&& heapBase)
	{
		SwapHeap&& heap = dynamic_cast<SwapHeap&&>(heapBase);
		root_ = mergeNodes(root_, heap.root_);
		heap.root_ = nullptr;
	}

	virtual bool empty() const
	{
		return root_ == nullptr;
	}

	virtual int getMin() const
	{
		return root_ == nullptr ? INT_MAX : root_->key;
	}

	virtual int extractMin()
	{
		int key = root_->key;
		Node* left = dynamic_cast<Node*>(root_->left);
		Node* right = dynamic_cast<Node*>(root_->right);
		root_->left = nullptr;
		root_->right = nullptr;
		delete root_;
		root_ = mergeNodes(left, right);
		return key;
	}

	virtual ~SwapHeap()
	{
		delete root_;
	}
};

typedef SwapHeap<SkewHeapNode> SkewHeap;
typedef SwapHeap<LeftHeapNode> LeftHeap;

#endif
