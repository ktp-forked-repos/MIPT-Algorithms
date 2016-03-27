#ifndef VECTORSTRUCT_H
#define VECTORSTRUCT_H

#include "iStruct.h"

class VectorStruct : public IStruct
{
  private:
	std::vector<int> a;

  public:
	virtual ll getSum(size_t l, size_t r) const
	{
		return accumulate(a.begin() + l, a.end() + r, 0LL);
	};

	virtual void insert(size_t i, int x)
	{
		a.insert(a.begin() + i, x);
	};

	virtual void set(size_t i, int x)
	{
		a[i] = x;
	};

	virtual void add(size_t l, size_t r, int x)
	{
		std::transform(a.begin(), a.end(), a.begin(), [x](int element) { return element + x; });
	};

	virtual void nextPermutation(size_t l, size_t r)
	{
		std::next_permutation(a.begin() + l, a.begin() + r + 1);
	};
};

#endif // VECTORSTRUCT_H
