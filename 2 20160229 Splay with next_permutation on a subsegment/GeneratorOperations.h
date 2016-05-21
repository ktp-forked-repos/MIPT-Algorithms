//
// Created by dima on 29.02.16.
//

#ifndef IDEA_20160229_SPLAY_OPERATIONS_H
#define IDEA_20160229_SPLAY_OPERATIONS_H

#include "base.h"
#include "IGenerator.h"
#include "IStruct.h"

class AbstractOperation {
public:
	virtual int getSizeChange() {
		return 0;
	}

	virtual void perform(IStruct *s, std::vector<ll> &result) = 0;

	virtual ~AbstractOperation() {
	}
};

class AbstractOperationWithLR : public AbstractOperation {
protected:
	size_t l;
	size_t r;

	AbstractOperationWithLR(IGenerator *generator) {
		generator->createRandomLR(l, r);
	}
};

class AbstractOperationWithIndexAndValue : public AbstractOperation {
protected:
	size_t i;
	int x;

	AbstractOperationWithIndexAndValue(IGenerator *generator, bool isIndexInsert = false)
			: i(generator->getRandomI(isIndexInsert))
			  , x(generator->getRandomValue()) {
	}
};

class OperationGetSum : public AbstractOperationWithLR {
public:
	OperationGetSum(IGenerator *generator) : AbstractOperationWithLR(generator) {
	}

	virtual void perform(IStruct *s, std::vector<ll> &result) {
		result.push_back(s->getSum(l, r));
	}
};

class OperationInsert : public AbstractOperationWithIndexAndValue {
public:
	OperationInsert(IGenerator *generator) : AbstractOperationWithIndexAndValue(generator, true) {
	}

	virtual int getSizeChange() {
		return 1;
	}

	virtual void perform(IStruct *s, std::vector<ll> &result) {
		s->insert(i, x);
	}
};

class OperationSet : public AbstractOperationWithIndexAndValue {
public:
	OperationSet(IGenerator *generator) : AbstractOperationWithIndexAndValue(generator) {
	}

	virtual void perform(IStruct *s, std::vector<ll> &result) {
		s->set(i, x);
	}
};

class OperationNextPermutation : public AbstractOperationWithLR {
public:
	OperationNextPermutation(IGenerator *generator) : AbstractOperationWithLR(generator) {
	}

	virtual void perform(IStruct *s, std::vector<ll> &result) {
		s->nextPermutation(l, r);
	}
};

class OperationAdd : public AbstractOperationWithLR {
public:
	int x;

	// если не ограничивать прибавление на подотрезке,
	// то уже через несколько операций возникнет переполнение, которое приведёт к ошибкам
	OperationAdd(IGenerator *generator) : AbstractOperationWithLR(generator)
			//, x(generator->getRandomValue()) {
	                                      , x(rand() % 100) {
	}

	virtual void perform(IStruct *s, std::vector<ll> &result) {
		s->add(l, r, x);
	}
};

enum OperationType {
	INSERT, GET_SUM, SET, NEXT_PERMUTATION, ADD, OPERATION_TYPE_NUMBER
};

#endif //IDEA_20160229_SPLAY_OPERATIONS_H
