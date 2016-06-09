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

void SmartObject::constructorCall() {
	if (GarbageCollector::debug)
		printf("constructor %20p\n", this);
	GarbageCollector::getInstance().registerCallConstructor(this);
}

void *SmartObject::operator new(size_t size) {
	auto pointer = ::operator new(size);
	//printf("new %p %zu\n", pointer, size);
	GarbageCollector::getInstance().registerCallOperatorNew(pointer, size, false);
	return pointer;
}

void *SmartObject::operator new[](size_t size) {
	auto pointer = ::operator new[](size);
	//printf("new[] %p %zu\n", pointer, size);
	GarbageCollector::getInstance().registerCallOperatorNew(pointer, size, true);
	return pointer;
}

void SmartObject::operator delete(void *pointer, size_t size) {
	//printf("delete %p %zu\n", pointer, size);
	GarbageCollector::getInstance().registerCallOperatorDelete(pointer, size);
	::operator delete(pointer);
}

void SmartObject::operator delete[](void *pointer, size_t size) {
	//printf("delete[] %p %zu\n", pointer, size);
	GarbageCollector::getInstance().registerCallOperatorDelete(pointer, size);
	::operator delete[](pointer);
}

SmartObject::~SmartObject() {
	if (GarbageCollector::debug)
		printf("destructor %21p\n", this);
	GarbageCollector::getInstance().registerCallDestructor(this);
}