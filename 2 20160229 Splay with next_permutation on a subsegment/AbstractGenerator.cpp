//
// Created by dima on 29.02.16.
//

#include "AbstractGenerator.h"

AbstractGenerator::AbstractGenerator(size_t n, int rangeMin, int rangeMax) : currentSize(0), n(n), rangeMin(rangeMin), rangeMax(rangeMax) {
}

void AbstractGenerator::add(AbstractOperation *operation) {
	if (result.size() < n) {
		result.push_back(operation);
		currentSize += operation->getSizeChange();
	}
}

void AbstractGenerator::createRandomLR(size_t &l, size_t &r) {
	l = getRandomI();
	r = getRandomI();
	if (l > r)
		std::swap(l, r);
}

size_t AbstractGenerator::getRandomI(bool isIndexInsert) {
	return rand() % (currentSize + isIndexInsert);
}

// [minRange, maxRange]
int AbstractGenerator::getRandomValue() {
	return rangeMin + rand() % (rangeMax - rangeMin + 1);
}