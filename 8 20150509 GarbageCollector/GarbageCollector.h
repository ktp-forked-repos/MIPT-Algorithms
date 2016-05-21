#ifndef GARBAGECOLLECTOR_GARBAGECOLLECTOR_H
#define GARBAGECOLLECTOR_GARBAGECOLLECTOR_H

#include "SmartObject.h"
#include <cstddef>
#include <map>
#include <set>

class GarbageCollector {

public:
	static GarbageCollector &getInstance();

	void registerCallOperatorNew(size_t size);

	void registerCallOperatorDelete(size_t size);

	void registerCallConstructor(SmartObject *smartObject);

	void registerCallDestructor(SmartObject *smartObject);

	void gc();

	static bool debug;

	//private:
	GarbageCollector() {
	}

	GarbageCollector(const GarbageCollector &root);

	GarbageCollector &operator=(const GarbageCollector &);

	void dfs(SmartObject *object);

	size_t sumSize = 0;
	std::set<SmartObject *> allObjects;

	static const size_t MAX_SUM_SIZE = 60000;

	~GarbageCollector();
};

#endif //GARBAGECOLLECTOR_GARBAGECOLLECTOR_H