#ifndef GARBAGECOLLECTOR_SMARTOBJECT_H
#define GARBAGECOLLECTOR_SMARTOBJECT_H

#include <vector>
#include <cstddef>
#include <map>

class SmartObject {
public:
	SmartObject();

	SmartObject(const SmartObject &other);

	SmartObject(SmartObject &&other);

	virtual std::vector<SmartObject *> pointers() const = 0; // Should be implemented by user

	void *operator new(size_t size);

	void operator delete(void *pointer, size_t size);

	virtual ~SmartObject();

	bool isHeap();

	bool isAccess;

	// хранит отрезки памяти, выделенной с помощью "operator new(size_ t size)"
	// отрезок [begin, end) хранится в виде segments[end] = begin;
	static std::map<void *, void *> segments;

private:
	void constructorCall();

	bool mIsHeap;
};

#endif //GARBAGECOLLECTOR_SMARTOBJECT_H