#ifndef ADVANCED_SUITE_H_
#define ADVANCED_SUITE_H_

#include "operationType.h"

int* getNewArray(int* array, size_t indexBegin, size_t size, size_t newIndexBegin, size_t newSize)
{
	int* newArray = new int[newSize];
	std::copy(array + indexBegin, array + indexBegin + size, newArray + newIndexBegin);
	delete[] array;
	return newArray;
}

void check(std::vector<OperationType> operations)
{
	MyDeque<int> d;
	int *array = new int[0];
	size_t size = 0;
	for (OperationType operation : operations)
	{
		bool isPop = (operation == POP_BACK || operation == POP_FRONT);
		if (size == 0 && isPop)
			continue;
		int x = rand();
		switch (operation)
		{
			case POP_BACK:
				d.pop_back();
				array = getNewArray(array, 0, size - 1, 0, size - 1);
				break;
			case POP_FRONT:
				d.pop_front();
				array = getNewArray(array, 1, size - 1, 0, size - 1);
				break;
			case PUSH_BACK:
				d.push_back(x);
				array = getNewArray(array, 0, size, 0, size + 1);
				array[size] = x;
				break;
			case PUSH_FRONT:
				d.push_front(x);
				array = getNewArray(array, 0, size, 1, size + 1);
				array[0] = x;
				break;
		}
		if (isPop)
			--size;
		else
			++size;

		BOOST_CHECK_EQUAL(d.size(), size);
		BOOST_CHECK_EQUAL(d.empty(), size == 0);
		if (size > 0)
		{
			BOOST_CHECK_EQUAL(d.front(), array[0]);
			BOOST_CHECK_EQUAL(d.back(), array[size - 1]);
		}
		for (size_t i = 0; i < size; ++i)
			BOOST_CHECK_EQUAL(d[i], array[i]);
	}
	delete[] array;
}

BOOST_AUTO_TEST_SUITE (AdvancedSuite)

const size_t NUMBER_OPERATIONS = 1000;

BOOST_AUTO_TEST_CASE(AdvancedTest1)
{
	srand(time(0));
	check (RandomGenerator().get(NUMBER_OPERATIONS));
	check (FullIncreaseDecreaseCycle().get(NUMBER_OPERATIONS));
	check (FewFullIncreaseDecreaseCycles().get(NUMBER_OPERATIONS));
	check (PartIncreaseDecreaseCycle().get(NUMBER_OPERATIONS));
}

BOOST_AUTO_TEST_CASE(AdvancedTest2)
{
	MyDeque<int> deque;
	for (size_t i = 0; i < 1000; ++i)
	deque.push_front(rand());

	MyDeque<int> deque2(deque);
	for (MyDeque<int>::iterator it = deque.begin(); it != deque.end(); ++it)
	BOOST_CHECK_EQUAL(*it, deque2[it - deque.begin()]);

	std::sort(deque.begin(), deque.end());
	std::sort(deque2.rbegin(), deque2.rend());

	for (size_t i = 1; i < deque.size(); ++i)
	BOOST_CHECK_LE(deque[i - 1], deque[i]);
	for (size_t i = 1; i < deque.size(); ++i)
	BOOST_CHECK_GE(deque2[i - 1], deque2[i]);

	MyDeque<int>::iterator iterator = deque.begin();
	MyDeque<int>::reverse_iterator iterator2 = deque2.rbegin();
	for (; iterator != deque.end(); ++iterator, ++iterator2)
	BOOST_CHECK_EQUAL(*iterator, *iterator2);
}

BOOST_AUTO_TEST_SUITE_END()

#endif
