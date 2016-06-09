#include "SmartObject.h"
#include "A.h"
#include "GarbageCollector.h"
#include <iostream>
#include <cassert>

void runAllTests();

int main2() {
	//runAllTests();
	A **as = new A *[3];
	for (int i = 0; i < 3; ++i)
		as[i] = new A[3];
	printf("ОК %.3f seconds\n", clock() / static_cast<float>(CLOCKS_PER_SEC));
	return 0;
}

// базовый
void simpleTest0() {
	A a1;
	new A();
	A a2;
	new A();
	GarbageCollector::getInstance().gc();
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

void simpleTest4() {
	A *as = new A[10];
}

void simpleTest5() {
	A *as = new A[0];
}

void simpleTest6() {
	A a;
	A *as1 = new A[3];
	a.add(&as1[0]);
	A *as2 = new A[3];
	a.add(&as2[0]);
	as1[1].add(&as2[2]);
	as2[1].add(&as1[2]);
}

void test0() {
	A a;
	A *a1 = new A();
	a.add(a1);
	A *a2 = new A();
	a.add(a2);
	a1->add(a2);
	a2->add(a1);
}

void test1(int number) {
	std::vector<A> as;
	for (size_t i = 0; i < number; ++i) {
		if (rand() % 2) {
			new A();
		} else {
			as.push_back(A());
		}
	}
}

struct ComplexTest : public SmartObject {
	enum Operation {
		CREATE_OBJECT_STACK,
		CREATE_OBJECT_HEAP,
		ADD_STACK_TO_STACK,
		ADD_STACK_TO_HEAP,
		ADD_HEAP_TO_STACK,
		ADD_HEAP_TO_HEAP,
		CREATE_ARRAY,
		ADD_ARRAY_TO_ARRAY,
		OPERATION_TYPE_NUMBER,
		ADD_ARRAY_TO_STACK,
		ADD_ARRAY_TO_HEAP,
		ADD_STACK_TO_ARRAY,
		ADD_HEAP_TO_ARRAY,
	};

	std::vector<A> stack;
	std::vector<A *> heap;
	std::vector<std::pair<A *, size_t>> arrays;

	template<typename T1, typename T2, typename Lambda>
	void add(T1 from, T2 to, Lambda lambda) {
		if (from.empty() || to.empty())
			return;
		size_t iFrom = rand() % from.size();
		size_t iTo = rand() % to.size();
		lambda(iFrom, iTo);
	}

	template<typename T, typename Lambda>
	void addToArray(T from, Lambda lambda) {
		if (from.empty() || arrays.empty())
			return;
		size_t iFrom = rand() % from.size();
		size_t iTo = rand() % arrays.size();
		auto &array = arrays[iTo];
		if (array.second > 0)
			lambda(iFrom, arrays[iTo].first[rand() % array.second]);
	}

	void test(size_t numberTests) {
		stack.reserve(numberTests);
		for (size_t i = 0; i < numberTests; ++i) {
			Operation operation = static_cast<Operation>(rand() % OPERATION_TYPE_NUMBER);
			switch (operation) {
				case CREATE_OBJECT_STACK:
					stack.push_back(A());
					break;
				case CREATE_OBJECT_HEAP:
					heap.push_back(new A());
					break;
				case CREATE_ARRAY: {
					size_t size = rand() % 20;
					arrays.emplace_back(new A[size], size);
					break;
				}
				case ADD_STACK_TO_STACK:
					add(stack, stack, [this](size_t from, size_t to) { stack[to].add(&stack[from]); });
					break;
				case ADD_STACK_TO_HEAP:
					add(stack, heap, [this](size_t from, size_t to) { heap[to]->add(&stack[from]); });
					break;
				case ADD_HEAP_TO_STACK:
					add(heap, stack, [this](size_t from, size_t to) { stack[to].add(heap[from]); });
					break;
				case ADD_HEAP_TO_HEAP:
					add(heap, heap, [this](size_t from, size_t to) { heap[to]->add(heap[from]); });
					break;
				case ADD_ARRAY_TO_ARRAY:
					addToArray(arrays, [this](size_t from, A &to) { to.add(arrays[from]); });
					break;
				case ADD_ARRAY_TO_STACK:
					add(arrays, stack, [this](size_t from, size_t to) { stack[to].add(arrays[from]); });
					break;
				case ADD_ARRAY_TO_HEAP:
					add(arrays, heap, [this](size_t from, size_t to) { heap[to]->add(arrays[from]); });
					break;
				case ADD_STACK_TO_ARRAY:
					addToArray(stack, [this](size_t from, A &to) { to.add(&stack[from]); });
					break;
				case ADD_HEAP_TO_ARRAY:
					addToArray(heap, [this](size_t from, A &to) { to.add(heap[from]); });
					break;
				case OPERATION_TYPE_NUMBER:
					assert(0);
					break;
			}
			if (rand() % 100 == 0) {
				GarbageCollector::getInstance().gc();
			}
		}
	}

	virtual std::vector<SmartObject *> pointers() const override {
		std::vector<SmartObject *> pointers;
		for (A *pointer : heap) {
			pointers.push_back(pointer);
		}
		for (auto pair : arrays) {
			if (pair.second > 0) {
				pointers.push_back(pair.first);
			}
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
	simpleTest4();
	simpleTest5();
	simpleTest6();

	test0();
	for (size_t i = 0; i < 100; ++i) {
		test1(100);
	}

	for (size_t i = 0; i < 100; ++i) {
		ComplexTest().test(100);
	}
	return;
}
