#ifndef GARBAGECOLLECTOR_GARBAGECOLLECTOR_H
#define GARBAGECOLLECTOR_GARBAGECOLLECTOR_H

#include "SmartObject.h"
#include "Segment.h"
#include <cstddef>
#include <set>

class GarbageCollector {
public:
	static GarbageCollector &getInstance();

	void registerCallOperatorNew(void *pointer, size_t size, bool isArray);

	void registerCallOperatorDelete(void *pointer, size_t size);

	void registerCallConstructor(SmartObject *smartObject);

	void registerCallDestructor(SmartObject *smartObject);

	void gc();

	static bool debug;

private:
	GarbageCollector() {
	}

	GarbageCollector(const GarbageCollector &root);

	GarbageCollector &operator=(const GarbageCollector &);

	void dfs(SmartObject *object);

	int sumSize;
	std::vector<SmartObject *> allObjects;
	std::vector<SmartObject *> deletedObjects;
	std::vector<Segment> allSegments;
	std::vector<Segment> deletedSegments;

	~GarbageCollector();

	static const size_t MAX_SUM_SIZE = 1200000;
};

#endif //GARBAGECOLLECTOR_GARBAGECOLLECTOR_H