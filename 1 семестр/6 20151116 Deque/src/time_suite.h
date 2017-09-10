#ifndef TIME_SUITE_H_
#define TIME_SUITE_H_

#include "operationType.h"

BOOST_AUTO_TEST_SUITE (TimeSuite)

void run(std::vector<OperationType> operations)
{
	MyDeque<int> d;
	for (OperationType operation : operations)
	{
		bool isPop = (operation == POP_BACK || operation == POP_FRONT);
		if (d.size() == 0 && isPop)
			continue;
		switch (operation)
		{
			case POP_BACK:
				d.pop_back();
				break;
			case POP_FRONT:
				d.pop_front();
				break;
			case PUSH_BACK:
				d.push_back(rand());
				break;
			case PUSH_FRONT:
				d.push_front(rand());
				break;
		}
	}
}

double getFactor(std::vector<OperationType> operations, size_t numberRepeats)
{
	for (size_t i = 0; i < 5; ++i)
		run(operations);
	auto start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < numberRepeats; ++i)
		run(operations);
	auto end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast < std::chrono::microseconds > (end - start).count() / static_cast<double>(operations.size() * numberRepeats);
}

const size_t NUMBER_OPERATIONS_BASE = 10000;
const size_t NUMBER_OPERATIONS = NUMBER_OPERATIONS_BASE * 100;

const size_t NUMBER_REPEATS_BASE = 20;
const size_t NUMBER_REPEATS = 20;

void test(Generator* generator)
{
	double factorBase = getFactor(generator->get(NUMBER_OPERATIONS_BASE), NUMBER_REPEATS_BASE);
	double factor = getFactor(generator->get(NUMBER_OPERATIONS), NUMBER_REPEATS);
	printf("%f %f %f\n", factorBase, factor, factor / factorBase);
	BOOST_CHECK_LE(factor, factorBase * 1.1);
	delete generator;
}

BOOST_AUTO_TEST_CASE(TimeTestRandom)
{
	test (new RandomGenerator());
	test (new FullIncreaseDecreaseCycle());
	test (new FewFullIncreaseDecreaseCycles());
	test (new PartIncreaseDecreaseCycle());
	printf("%.3f seconds\n", clock() / static_cast<double>(CLOCKS_PER_SEC));
}

BOOST_AUTO_TEST_SUITE_END()

#endif
