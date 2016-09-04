#ifndef IDEA_20160229_SPLAY_SPLAYOPERATIONS_H
#define IDEA_20160229_SPLAY_SPLAYOPERATIONS_H

#include "SplayOperations.h"

ll getSum(SplayNode *node) {
	return node == nullptr ? 0 : node->sum;
}

size_t getSize(SplayNode *node) {
	return node == nullptr ? 0 : node->size;
}

void update(SplayNode *node) {
	if (node != nullptr) {
		node->update();
	}
}

void push(SplayNode *node) {
	assert(node != nullptr);
	node->push();
}

void deAttach(SplayNode *node) {
	if (node == nullptr || node->parent == nullptr) {
		return;
	}
	SplayNode *parent = node->parent;
	assert(!node->needPush);
	assert(!parent->needPush);
	(node->isLeft() ? parent->left : parent->right) = nullptr;
	node->parent = nullptr;
	update(parent);
}

void rotate(SplayNode *node) {
	SplayNode *parent = node->parent;
	assert(parent != nullptr);
	SplayNode *parent2 = parent->parent;

	assert(!node->needPush);
	assert(!parent->needPush);
	assert(parent2 == nullptr || !parent2->needPush);

	bool isLeft = node->isLeft();
	bool isParentLeft = parent2 == nullptr ? false : parent->isLeft();
	deAttach(node);
	deAttach(parent);
	SplayNode *child = isLeft ? node->right : node->left;
	if (child != nullptr) {
		push(child);
		deAttach(child);
	}

	parent->set(child, isLeft);
	node->set(parent, !isLeft);
	if (parent2 != nullptr) {
		parent2->set(node, isParentLeft);
	}
}

void splay(SplayNode *node) {
	while (1) {
		assert(!node->needPush);
		SplayNode *parent = node->parent;
		if (parent == nullptr) {
			break;
		}
		assert(!parent->needPush);
		SplayNode *parent2 = parent->parent;

		if (parent2 != nullptr) {
			assert(!parent2->needPush);
		}

		if (parent2 == nullptr) {
			// zig
			rotate(node);
		}
		else if (node->isLeft() == parent->isLeft()) {
			// zig-zig
			rotate(parent);
			rotate(node);
		}
		else {
			// zig-zag
			rotate(node);
			rotate(node);
		}
	}
}

SplayNode *find(SplayNode *node, size_t i) {
	if (node == nullptr) {
		return nullptr;
	}
	push(node);
	size_t sizeLeft = getSize(node->left);
	if (sizeLeft == i) {
		splay(node);
		return node;
	}
	else if (sizeLeft < i) {
		return find(node->right, i - sizeLeft - 1);
	}
	else {
		return find(node->left, i);
	}
}

std::pair<SplayNode *, SplayNode *> split(SplayNode *node, size_t size) {
	if (node == nullptr) {
		return {nullptr, nullptr};
	}
	assert(node->parent == nullptr);
	push(node);
	if (size == 0) {
		return {nullptr, node};
	}
	SplayNode *rootLeft = find(node, size - 1);
	SplayNode *rightChild = rootLeft->right;
	if (rightChild != nullptr) {
		rightChild->push();
	}
	deAttach(rightChild);
	assert(getSize(rootLeft) == size);
	return {rootLeft, rightChild};
}

SplayNode *findMax(SplayNode *node) {
	if (node == nullptr)
		return nullptr;
	assert(node->size != 0);
	return find(node, node->size - 1);
}

SplayNode *merge(SplayNode *left, SplayNode *right) {
	assert(left == nullptr || left->parent == nullptr);
	assert(right == nullptr || right->parent == nullptr);
	if (left == nullptr || right == nullptr) {
		return left == nullptr ? right : left;
	}
	push(left);
	push(right);
	SplayNode *leftMax = findMax(left);
	leftMax->setRight(right);
	return leftMax;
}

SplayNode *findFirstLarger(SplayNode *node, int value) {
	SplayNode *result = nullptr;
	SplayNode *debug = node;
	assert(node != nullptr);
	while (node != nullptr) {
		push(node);
		if (node->value <= value)
			node = node->left;
		else {
			result = node;
			node = node->right;
		}
	}
	assert(result != nullptr);
	splay(result);
	return result;
}

void checkParentsPushing(SplayNode *node) {
	while (node != nullptr) {
		assert(!node->needPush);
		node = node->parent;
	}
}

SplayNode *nextPermutation(SplayNode *node) {
	assert(node != nullptr);
	push(node);
	update(node);
	size_t sizeSuffixDec = node->sizeSuffixDec;
	if (sizeSuffixDec == node->size) {
		node->setReverse();
		return node;
	}
	auto p = split(node, node->size - sizeSuffixDec - 1);
	auto p2 = split(p.second, 1);
	int value1 = p2.first->value;
	SplayNode *firstLarger = findFirstLarger(p2.second, value1);
	int value2 = firstLarger->value;
	firstLarger->setValue(value1);
	p2.first->setValue(value2);
	firstLarger->setReverse();
	p.second = merge(p2.first, firstLarger);
	return merge(p.first, p.second);
}

#endif //IDEA_20160229_SPLAY_SPLAYOPERATIONS_H
