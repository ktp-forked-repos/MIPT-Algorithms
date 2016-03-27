//
// Created by dima on 29.02.16.
//

#include <algorithm>
#include "VectorStruct.h"

size_t VectorStruct::getSize() {
	return a_.size();
}

ll VectorStruct::getSum(size_t l, size_t r) {
	return std::accumulate(a_.begin() + l, a_.begin() + r + 1, 0LL);
};

void VectorStruct::insert(size_t i, int x) {
	a_.insert(a_.begin() + i, x);
};

void VectorStruct::set(size_t i, int x) {
	a_[i] = x;
};

void VectorStruct::nextPermutation(size_t l, size_t r) {
	std::next_permutation(a_.begin() + l, a_.begin() + r + 1);
};

void VectorStruct::add(size_t l, size_t r, int x) {
	std::transform(a_.begin() + l, a_.begin() + r + 1, a_.begin() + l, [x](int element) { return element + x; });
};
