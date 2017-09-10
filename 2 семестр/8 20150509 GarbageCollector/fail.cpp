#include "SmartObject.h"
#include <bits/stdc++.h>
 
class A;
 
class B : public SmartObject {
public:
	A *pointer;
 
	virtual std::vector<SmartObject *> pointers() const {
		std::vector<SmartObject *> pointers;
		pointers.push_back((SmartObject *) pointer);
		return pointers;
	}
};
 
class A : public SmartObject {
public:
	std::vector<B> bs;
 
	A() {
		bs.resize(1);
		bs[0].pointer = this;
	}
 
	virtual std::vector<SmartObject *> pointers() const {
		return {};
	}
};
 
int main() {
	new A();
	return 0;
}
