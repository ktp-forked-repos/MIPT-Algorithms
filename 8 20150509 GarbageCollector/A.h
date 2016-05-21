#ifndef GARBAGECOLLECTOR_A_H
#define GARBAGECOLLECTOR_A_H

#include <iostream>
#include "SmartObject.h"

class A : public SmartObject {

public:
	virtual std::vector<SmartObject *> pointers() const override {
		std::vector<SmartObject *> pointers;
		for (A *pointer : heapPointers) {
			pointers.push_back(pointer);
		}
		return pointers;
	};

	std::vector<A *> heapPointers;

	A &add(A *a) {
		heapPointers.push_back(a);
		return *this;
	}
};

#endif //GARBAGECOLLECTOR_A_H