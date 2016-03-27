#ifndef ISTRUCT_H
#define ISTRUCT_H

#include "base.h"

class IStruct
{
  public:
	virtual ll getSum(size_t l, size_t r) const = 0;
	virtual void insert(size_t i, int x) = 0;
	virtual void set(size_t i, int x) = 0;
	virtual void add(size_t l, size_t r, int x) = 0;
	virtual void nextPermutation(size_t l, size_t r) = 0;

	virtual ~IStruct()
	{
	}
};

#endif // ISTRUCT_H
