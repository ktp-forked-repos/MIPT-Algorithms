//
// Created by dima on 29.02.16.
//

#ifndef IDEA_20160229_SPLAY_VECTORSTRUCT_H
#define IDEA_20160229_SPLAY_VECTORSTRUCT_H

#include "base.h"
#include "IStruct.h"

class VectorStruct : public IStruct {
private:
	std::vector<int> a_;

public:
	virtual size_t getSize();

	virtual ll getSum(size_t l, size_t r);

	virtual void insert(size_t i, int x);

	virtual void set(size_t i, int x);

	virtual void add(size_t l, size_t r, int x);

	virtual void nextPermutation(size_t l, size_t r);
};

#endif //IDEA_20160229_SPLAY_VECTORSTRUCT_H
