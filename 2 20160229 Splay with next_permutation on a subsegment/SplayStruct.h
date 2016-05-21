#ifndef IDEA_20160229_SPLAY_SPLAYSTRUCT_H
#define IDEA_20160229_SPLAY_SPLAYSTRUCT_H

#include "base.h"
#include "SplayNode.h"
#include "IStruct.h"

class SplayStruct : public IStruct {
private:
	SplayNode *root_;

public:
	SplayStruct();

	virtual size_t getSize();

	virtual ll getSum(size_t l, size_t r);

	virtual void insert(size_t i, int x);

	virtual void set(size_t i, int x);

	virtual void add(size_t l, size_t r, int x);

	virtual void nextPermutation(size_t l, size_t r);

	virtual ~SplayStruct();
};

#endif //IDEA_20160229_SPLAY_SPLAYSTRUCT_H
