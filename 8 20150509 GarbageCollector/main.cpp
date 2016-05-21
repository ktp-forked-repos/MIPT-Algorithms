#include <iostream>
#include <cassert>
#include "SmartObject.h"
#include "A.h"
#include "GarbageCollector.h"

void runAllTests();

int main() {
	runAllTests();
	return 0;
}

// базовый
void simpleTest0() {
	A a1;
	new A();
	A a2;
	new A();
}

// конструкторы копирования
void simpleTest1() {
	A a1;
	A a2 = a1;
	A a3 = A();
}

// конструкторы копирования внутри вектора
void simpleTest2() {
	std::vector<A> as;
	as.push_back(A());
	new A();
	new A();
	as.push_back(A());
	as.push_back(A());
	new A();
	new A();
	as.push_back(A());
	as.push_back(A());
	as.push_back(A());
	new A();
	as.push_back(A());
	new A();
}

void simpleTest3() {
	A a;
	A *pa = new A();
	a.add(pa);
}

// массив
void simpleTest4() {
	A *as = new A[10];
}

void test0() {
	std::vector<A> as;
	for (size_t i = 0; i < 10000; ++i) {
		if (rand() % 2) {
			new A();
		} else {
			as.push_back(A());
		}
	}
}

void test1() {
	A a;
	A *a1 = new A();
	A *a2 = new A();
	a.add(a1);
	a.add(a2);
	a1->add(a2);
	a2->add(a1);
}

enum Operation {
	CREATE_OBJECT_STACK,
	CREATE_OBJECT_HEAP,
	ADD_STACK_TO_STACK,
	ADD_STACK_TO_HEAP,
	ADD_HEAP_TO_STACK,
	ADD_HEAP_TO_HEAP,
};

struct ComplexTest : public SmartObject {
	std::vector<A> stack;
	std::vector<A *> heap;

	void test(size_t numberTests) {
		bool debug = 0;
		stack.reserve(numberTests);
		for (size_t i = 0; i < numberTests; ++i) {
			Operation operation = static_cast<Operation>(rand() % 6);
			if (stack.empty())
				operation = CREATE_OBJECT_STACK;
			else if (heap.empty())
				operation = CREATE_OBJECT_HEAP;
			switch (operation) {
				case CREATE_OBJECT_STACK: {
					stack.push_back(A());
					if (debug)
						printf("stack.push_back(A());\n");
					break;
				}
				case CREATE_OBJECT_HEAP: {
					heap.push_back(new A());
					if (debug)
						printf("heap.push_back(new A());\n");
					break;
				}
				case ADD_STACK_TO_STACK: {
					size_t iFrom = rand() % stack.size();
					size_t iTo = rand() % stack.size();
					stack[iFrom].add(&stack[iTo]);
					if (debug)
						printf("stack[%d].add(stack[%d]);\n", iFrom, iTo);
					break;
				}
				case ADD_STACK_TO_HEAP: {
					size_t iFrom = rand() % stack.size();
					size_t iTo = rand() % heap.size();
					stack[iFrom].add(heap[iTo]);
					if (debug)
						printf("stack[%d].add(heap[%d]);\n", iFrom, iTo);
					break;
				}
				case ADD_HEAP_TO_STACK: {
					size_t iFrom = rand() % heap.size();
					size_t iTo = rand() % stack.size();
					heap[iFrom]->add(&stack[iTo]);
					if (debug)
						printf("heap[%d]->add(stack[%d]);\n", iFrom, iTo);
					break;
				}
				case ADD_HEAP_TO_HEAP: {
					size_t iFrom = rand() % heap.size();
					size_t iTo = rand() % heap.size();
					heap[iFrom]->add(heap[iTo]);
					if (debug)
						printf("heap[%d]->add(heap[%d]);\n", iFrom, iTo);
					break;
				}
			}
		}

		if (debug) {
			printf("stack:\n");
			for (A &a : stack)
				printf("%p\n", &a);
			printf("heap:\n");
			for (A *a : heap)
				printf("%p\n", a);
		}
	}

	virtual std::vector<SmartObject *> pointers() const override {
		std::vector<SmartObject *> pointers;
		for (A *pointer : heap) {
			pointers.push_back(pointer);
		}
		return pointers;
	}
};

void runAllTests() {
	srand(time(0));
	simpleTest0();
	simpleTest1();
	simpleTest2();
	simpleTest3();

	test0();
	test1();

	for (size_t i = 0; i < 10000; ++i) {
		ComplexTest().test(7);
	}
}