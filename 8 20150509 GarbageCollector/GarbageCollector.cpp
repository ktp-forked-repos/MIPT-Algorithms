#include <set>
#include <cassert>
#include <iostream>
#include <algorithm>
#include "GarbageCollector.h"

GarbageCollector &GarbageCollector::getInstance() {
	static GarbageCollector theSingleInstance;
	return theSingleInstance;
}

void GarbageCollector::registerCallOperatorNew(size_t size) {
	sumSize += size;
	if (sumSize >= MAX_SUM_SIZE) {
		gc();
	}
	if (sumSize >= MAX_SUM_SIZE) {
		throw std::bad_alloc();
	}
}

void GarbageCollector::registerCallOperatorDelete(size_t size) {
	sumSize -= size;
}

void GarbageCollector::registerCallConstructor(SmartObject *smartObject) {
	assert(allObjects.find(smartObject) == allObjects.end());
	allObjects.insert(smartObject);
}

void GarbageCollector::registerCallDestructor(SmartObject *smartObject) {
	assert(allObjects.find(smartObject) != allObjects.end());
	allObjects.erase(smartObject);
}

void GarbageCollector::gc() {
	if (debug) {
		printf("\n");
		printf("==================================\n");
		printf("Garbage Collector (size = %d)\n", sumSize);

		printf("\tall:\n");
		for (SmartObject *object : allObjects)
			printf("%32p %d\n", object, object->isHeap());
		printf("\n");
	}

	for (SmartObject *object : allObjects)
		object->isAccess = false;
	for (SmartObject *object : allObjects)
		if (!object->isHeap())
			dfs(object);
	std::vector<SmartObject *> objectsToDelete;
	for (SmartObject *object : allObjects)
		if (!object->isAccess)
			objectsToDelete.push_back(object);
	std::set<SmartObject *> copyAllObjects = allObjects;
	for (SmartObject *object : objectsToDelete)
		delete object;
	for (SmartObject *object : allObjects)
		assert(object->isAccess);

	if (debug) {
		printf("Done (size = %d)\n", sumSize);
		printf("==================================\n");
		printf("\n");
	}
}

void GarbageCollector::dfs(SmartObject *object) {
	if (object->isAccess) {
		return;
	}
	object->isAccess = true;
	if (debug)
		printf("find %27p\n", object);
	for (SmartObject *pointer : object->pointers()) {
		dfs(pointer);
	}
}

GarbageCollector::~GarbageCollector() {
	if (debug)
		printf("~GarbageCollector\n");
	gc();
	if (!allObjects.empty()) {
		printf("There are left objects!!!\n");
		printf("number = %zu\n", allObjects.size());
		printf("sumSize = %d\n", sumSize);
		for (SmartObject *object : allObjects)
			printf("%32p %d\n", object, object->isHeap());
	}
	assert(allObjects.empty());
	assert(sumSize == 0);
}

bool GarbageCollector::debug = 0;