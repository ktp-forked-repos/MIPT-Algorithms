#include "SmartObject.h"
#include "GarbageCollector.h"
#include <iostream>
#include <cassert>

SmartObject::SmartObject() {
	constructorCall();
}

SmartObject::SmartObject(const SmartObject &other) {
	constructorCall();
}

SmartObject::SmartObject(SmartObject &&other) {
	constructorCall();
}

bool SmartObject::isHeap() {
	return mIsHeap;
}

void SmartObject::constructorCall() {
	auto it = segments.upper_bound(this);
	mIsHeap = it != segments.end() && it->second <= this;
	if (GarbageCollector::debug)
		printf("constructor %20p %d\n", this, mIsHeap);
	GarbageCollector::getInstance().registerCallConstructor(this);
}

void *SmartObject::operator new(size_t size) {
	GarbageCollector::getInstance().registerCallOperatorNew(size);
	auto pointer = ::operator new(size);
	segments[pointer + size] = pointer;
	return pointer;
}

void SmartObject::operator delete(void *pointer, size_t size) {
	GarbageCollector::getInstance().registerCallOperatorDelete(size);
	::operator delete(pointer);
}

SmartObject::~SmartObject() {
	if (GarbageCollector::debug)
		printf("destructor %21p\n", this);
	GarbageCollector::getInstance().registerCallDestructor(this);
}

std::map<void *, void *> SmartObject::segments;