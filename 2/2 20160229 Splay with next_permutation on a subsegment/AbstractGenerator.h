//
// Created by dima on 29.02.16.
//

#ifndef IDEA_20160229_SPLAY_ABSTRACTGENERATOR_H
#define IDEA_20160229_SPLAY_ABSTRACTGENERATOR_H

#include "base.h"
#include "GeneratorOperations.h"
#include "IGenerator.h"

class AbstractGenerator : public IGenerator {
public:
	size_t currentSize;
	size_t n;
	int rangeMin;
	int rangeMax;
	std::vector<AbstractOperation *> result;

	AbstractGenerator(size_t n = 10, int rangeMin = 0, int rangeMax = 10);

	virtual std::vector<AbstractOperation *> generate() = 0;

	void add(AbstractOperation *operation);

	void createRandomLR(size_t &l, size_t &r);

	size_t getRandomI(bool isIndexInsert = false);

	// [minRange, maxRange]
	int getRandomValue();
};

#endif //IDEA_20160229_SPLAY_ABSTRACTGENERATOR_H
