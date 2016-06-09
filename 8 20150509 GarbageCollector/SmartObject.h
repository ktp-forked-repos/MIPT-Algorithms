#ifndef GARBAGECOLLECTOR_SMARTOBJECT_H
#define GARBAGECOLLECTOR_SMARTOBJECT_H

#include "Segment.h"
#include <vector>
#include <cstddef>

class SmartObject {
public:
	SmartObject();

	SmartObject(const SmartObject &other);

	SmartObject(SmartObject &&other);

	virtual std::vector<SmartObject *> pointers() const = 0; // Should be implemented by user

	void *operator new(size_t size);

	void *operator new[](size_t size);

	void operator delete(void *pointer, size_t size);

	void operator delete[](void *pointer, size_t size);

	virtual ~SmartObject();

	bool isAccess;
	SmartObject *left;
	SmartObject *right;

private:
	void constructorCall();
};

#endif //GARBAGECOLLECTOR_SMARTOBJECT_H