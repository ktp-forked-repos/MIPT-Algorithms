#ifndef SPLAY_H
#define SPLAY_H

#include "base.h"
#include "iStruct.h"

void update(SplayNode *node);

class SplayNode
{
  public:
	SplayNode *l;
	SplayNode *r;
	SplayNode *parent;
	size_t size;
	int value;
	ll sum;

	SplayNode(int value, SplayNode *parent = nullptr)
		: l(nullptr), r(nullptr), parent(parent), size(1), value(value), sum(value)
	{
	}

	bool isLeft()
	{
		assert(parent != nullptr);
		return this == parent->l;
	}

	void setLeft(SplayNode *child)
	{
		l = child;
		child->parent = this;
	}

	void setRight(SplayNode *child)
	{
		r = child;
		child->parent = this;
	}

	void set(SplayNode *child, bool isLeft)
	{
		(isLeft ? l : r) = child;
		child->parent = this;
	}

	void setValue(int value)
	{
		this->value = value;
		update(this);
	}

	~SplayNode()
	{
		delete l;
		delete r;
	}
};

void deAttach(SplayNode *node)
{
	if (node != nullptr && node->parent != nullptr) {
		(node->isLeft() ? node->parent->l : node->parent->r) = nullptr;
		node->parent = nullptr;
	}
}

void rotate(SplayNode *node)
{
	SplayNode *parent = node->parent;
	assert(parent != nullptr);
	SplayNode *parent2 = parent->parent;

	bool isLeft = node->isLeft();
	bool isParentLeft = parent->isLeft();
	deAttach(node);
	deAttach(parent);
	SplayNode *child = isLeft ? node->r : node->r;
	deAttach(child);

	parent->set(child, isLeft);
	node->set(parent, !isLeft);
	if (parent2 != nullptr) {
		parent2->set(node, isParentLeft);
	}
}

void splay(SplayNode *node)
{
	SplayNode *parent = node->parent;
	if (parent == nullptr) {
		return;
	}
	SplayNode *parent2 = parent->parent;
	// zig
	if (parent2 == nullptr) {
		rotate(node);
		return;
	}
	// zig-zig
	if (node->isLeft() == parent->isLeft()) {
		rotate(parent);
		rotate(node);
		return;
	}
	// zig-zag
	rotate(node);
	rotate(node);
}

SplayNode *find(SplayNode *node, size_t i)
{
	if (node == nullptr)
		return nullptr;
	size_t sizeLeft = getSize(node->l);
	if (sizeLeft == i)
		return node;
	else if (sizeLeft < i)
		return find(node->r, i - leftSize - 1);
	else
		return find(node->l, i);
}

std::pair<SplayNode *, SplayNode *> split(SplayNode *node, size_t size)
{
	SplayNode *rootLeft = find(node, size - 1);
	assert(rootLeft != nullptr);
	splay(rootLeft);
	SplayNode *rightChild = rootLeft->r;
	deAttach(rightChild);
	return { rootLeft, rightChild };
}

SplayNode *findMax(SplayNode *node)
{
	if (node == nullptr)
		return nullptr;
	while (node->r != nullptr)
		node = node->r;
	return node;
}

SplayNode *merge(SplayNode *left, SplayNode *right)
{
	if (left == nullptr || right == nullptr)
		return left == nullptr ? right : left;
	SplayNode *leftMax = findMax(left);
	splay(leftMax);
	leftMax->setRight(right);
	return leftMax;
}

ll getSum(SplayNode *node)
{
	return node == nullptr ? 0 : node->sum;
}

ll getSize(SplayNode *node)
{
	return node == nullptr ? 0 : node->size;
}

void update(SplayNode *node)
{
	if (node == nullptr) {
		return;
	}
	node->size = getSize(node->l) + 1 + getSize(node->r);
	node->sum = getSum(node->l) + 1 + getSum(node->r);
}

class SplayStruct : public IStruct
{
  private:
	SplayNode *root;

  public:
	SplayStruct()
		: root(nullptr)
	{
	}

	virtual ll get_sum(size_t l, size_t r)
	{
		auto p = split(root, l);
		auto p2 = split(p.second, r - l + 1);
		ll sum = getSum(p2.first);
		p.second = merge(p2.first, p2.second);
		root = merge(p.first, p.second);
		return sum;
	}

	virtual void insert(size_t i, int x)
	{
		auto p = split(root, i);
		p.first = merge(p.first, new SplayNode(x));
		root = merge(p.first, p.second);
	}

	virtual void set(size_t i, int x)
	{
		auto p = split(root, i);
		auto p2 = split(p.second, 1);
		assert(p2.first != nullptr);
		p2.first->setValue(x);
		p.second = merge(p2.first, p2.second);
		root = merge(p.first, p.second);
	}

	virtual void add(size_t l, size_t r, int x)
	{
	}

	virtual void next_permutation(size_t l, size_t r)
	{
	}

	virtual ~SplayStruct()
	{
		delete root;
	}
};

#endif // SPLAY_H
