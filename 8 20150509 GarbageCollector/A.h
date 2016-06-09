#ifndef GARBAGECOLLECTOR_A_H
#define GARBAGECOLLECTOR_A_H

#include <iostream>
#include "SmartObject.h"

class A : public SmartObject {

public:
	virtual std::vector<SmartObject *> pointers() const override {
		std::vector<SmartObject *> pointers;
		for (SmartObject *object : heapPointers) {
			pointers.push_back(object);
		}
		return pointers;
	};

	std::vector<A *> heapPointers;

	void add(A *a) {
		heapPointers.push_back(a);
	}

	void add(std::pair<A *, size_t> array) {
		if (array.second == 0)
			return;
		add(&array.first[rand() % array.second]);
	}
};

#endif //GARBAGECOLLECTOR_A_H