#ifndef GARBAGECOLLECTOR_SEGMENT_H
#define GARBAGECOLLECTOR_SEGMENT_H

#include <cstddef>

class SmartObject;

struct Segment {
	void *begin = nullptr;
	void *end = nullptr;
	SmartObject *leftmost = nullptr;
	SmartObject *rightmost = nullptr;
	bool isArray = false;

	Segment() {
	}

	Segment(void *begin, void *end, bool isArray = false) : begin(begin), end(end), isArray(isArray) {
	}

	bool operator<(const Segment &segment) const {
		return begin < segment.begin || (begin == segment.begin && end < segment.end);
	}

	size_t size() const {
		return reinterpret_cast<size_t>(end) - reinterpret_cast<size_t>(begin);
	}
};

#endif //GARBAGECOLLECTOR_SEGMENT_H
