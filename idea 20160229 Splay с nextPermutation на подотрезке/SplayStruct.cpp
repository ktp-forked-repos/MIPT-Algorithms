//
// Created by dima on 29.02.16.
//

#include "SplayStruct.h"
#include "SplayOperations.h"

SplayStruct::SplayStruct()
		: root_(nullptr) {
}

size_t SplayStruct::getSize() {
	return ::getSize(root_);
}

ll SplayStruct::getSum(size_t l, size_t r) {
	auto p = split(root_, l);
	auto p2 = split(p.second, r - l + 1);
	ll sum = ::getSum(p2.first);
	p.second = merge(p2.first, p2.second);
	root_ = merge(p.first, p.second);
	return sum;
}

void SplayStruct::insert(size_t i, int x) {
	auto p = split(root_, i);
	p.first = merge(p.first, new SplayNode(x));
	root_ = merge(p.first, p.second);
}

void SplayStruct::set(size_t i, int x) {
	SplayNode *node = find(root_, i);
	assert(node != nullptr);
	checkParentsPushing(node);
	splay(node);
	node->setValue(x);
	root_ = node;
}

void SplayStruct::nextPermutation(size_t l, size_t r) {
	auto p = split(root_, l);
	auto p2 = split(p.second, r - l + 1);
	p2.first = ::nextPermutation(p2.first);
	p.second = merge(p2.first, p2.second);
	root_ = merge(p.first, p.second);
}

void SplayStruct::add(size_t l, size_t r, int x) {
	auto p = split(root_, l);
	auto p2 = split(p.second, r - l + 1);
	p2.first->add(x);
	p.second = merge(p2.first, p2.second);
	root_ = merge(p.first, p.second);
}

SplayStruct::~SplayStruct() {
	delete root_;
}
