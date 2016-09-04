#include "GarbageCollector.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <map>

GarbageCollector &GarbageCollector::getInstance() {
	static GarbageCollector theSingleInstance;
	return theSingleInstance;
}

void GarbageCollector::registerCallOperatorNew(void *pointer, size_t size, bool isArray) {
	sumSize += size;
	if (sumSize >= MAX_SUM_SIZE) {
		gc();
	}
	if (sumSize >= MAX_SUM_SIZE) {
		throw std::bad_alloc();
	}
	allSegments.push_back({pointer, pointer + size, isArray});
}

void GarbageCollector::registerCallOperatorDelete(void *pointer, size_t size) {
	deletedSegments.push_back({pointer, pointer + size});
	sumSize -= size;
}

void GarbageCollector::registerCallConstructor(SmartObject *smartObject) {
	allObjects.push_back(smartObject);
}

void GarbageCollector::registerCallDestructor(SmartObject *smartObject) {
	deletedObjects.push_back(smartObject);
}

template<typename T>
void handleDeleted(std::vector<T> &all, std::vector<T> &deleted) {
	std::sort(all.begin(), all.end());
	std::sort(deleted.begin(), deleted.end());
	std::vector<T> newAll;
	std::set_difference(all.begin(), all.end(), deleted.begin(), deleted.end(), std::inserter(newAll, newAll.end()));
	all.swap(newAll);
	deleted.clear();
}

void GarbageCollector::gc() {
	if (debug) {
		printf("\n");
		printf("==================================\n");
		printf("Garbage Collector (size = %d)\n", sumSize);

		printf("\tall:\n");
		for (SmartObject *object : allObjects)
			printf("%32p %d\n", object, 0);
		printf("\n");
	}

	if (debug) {
		printf("allSegments\n");
		for (Segment segment : allSegments)
			printf("%p %p %d\n", segment.begin, segment.end, segment.isArray);
		printf("deletedSegments\n");
		for (Segment segment : deletedSegments)
			printf("%p %p %d\n", segment.begin, segment.end, segment.isArray);
	}

	handleDeleted(allObjects, deletedObjects);
	handleDeleted(allSegments, deletedSegments);

	std::map<void *, Segment> mapSegments;
	for (Segment segment : allSegments) {
		mapSegments[segment.end] = segment;
	}

	for (SmartObject *object : allObjects) {
		auto it = mapSegments.upper_bound(object);
		if (it == mapSegments.end() || it->second.begin > object) {
			// на стеке
			object->right = object->left = nullptr;
			continue;
		}
		Segment &segment = it->second;
		if (segment.leftmost == nullptr) {
			segment.leftmost = segment.rightmost = object;
			object->left = object->right = object;
		} else {
			SmartObject *leftmost = segment.leftmost;
			SmartObject *rightmost = segment.rightmost;
			assert(leftmost != nullptr);
			assert(rightmost != nullptr);
			if (object > rightmost) {
				object->right = rightmost->right;
				object->left = rightmost;
				rightmost->right = segment.rightmost = object;
			} else {
				assert(object < leftmost);
				object->left = leftmost->left;
				object->right = leftmost;
				leftmost->left = segment.leftmost = object;
			}
		}
	}

	for (SmartObject *object : allObjects) {
		object->isAccess = false;
	}
	for (SmartObject *object : allObjects) {
		if (object->left == nullptr) {
			dfs(object);
		}
	}

	if (debug)
		printf("delete\n");
	size_t numberDeletes = 0;
	for (auto it : mapSegments) {
		Segment &segment = it.second;
		if (segment.size() == 8) {
			++numberDeletes;
			SmartObject::operator delete[](segment.begin, segment.size());
			continue;
		}
		if (debug)
			printf("%p %p %d\n", segment.begin, segment.end, segment.isArray);
		assert(segment.leftmost != nullptr);
		if (!segment.leftmost->isAccess) {
			++numberDeletes;
			if (segment.isArray) {
				for (SmartObject *object = segment.leftmost, *next; object != segment.rightmost; object = next) {
					next = object->right;
					object->~SmartObject();
				}
				segment.rightmost->~SmartObject();
				SmartObject::operator delete[](segment.begin, segment.size());
			} else {
				assert(segment.leftmost == segment.rightmost);
				delete segment.leftmost;
			}
		}
	}
	assert(numberDeletes == deletedSegments.size());

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
	if (object->left != nullptr) {
		assert(object->right != nullptr);
		dfs(object->left);
		dfs(object->right);
	}
	for (SmartObject *pointer : object->pointers()) {
		dfs(pointer);
	}
}

GarbageCollector::~GarbageCollector() {
	if (debug)
		printf("~GarbageCollector\n");
	gc();
	if (allObjects.size() != deletedObjects.size()) {
		printf("There are left objects!!!\n");
		printf("number = %zu\n", allObjects.size());
		printf("sumSize = %d\n", sumSize);
		for (SmartObject *object : allObjects)
			printf("%32p %d\n", object, 1);
	}
	assert(allObjects.size() == deletedObjects.size());
	assert(allSegments.size() == deletedSegments.size());
	assert(sumSize == 0);
}

bool GarbageCollector::debug = 1;