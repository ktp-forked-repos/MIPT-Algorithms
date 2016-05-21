
#ifndef IDEA_20160229_SPLAY_ISTRUCT_H
#define IDEA_20160229_SPLAY_ISTRUCT_H

#include "base.h"

class IStruct {
public:
	virtual size_t getSize() = 0;

    virtual ll getSum(size_t l, size_t r) = 0;

    virtual void insert(size_t i, int x) = 0;

    virtual void set(size_t i, int x) = 0;

    virtual void add(size_t l, size_t r, int x) = 0;

    virtual void nextPermutation(size_t l, size_t r) = 0;

    virtual ~IStruct() {
    }
};


#endif //IDEA_20160229_SPLAY_ISTRUCT_H
