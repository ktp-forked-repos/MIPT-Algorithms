#ifndef TESTS_H_
#define TESTS_H_
#define BOOST_TEST_MAIN

#include "BinomialHeap.h"
#include "SwapHeap.h"
#include "SetHeap.h"
#include "PriorityQueueHeap.h"

template<class Heap>
void addHeap(std::vector<Heap*> &heaps, int key)
{
	heaps.push_back(new Heap(key));
}

template<class Heap>
void insert(std::vector<Heap*> &heaps, int index, int key)
{
	heaps[index]->insert(key);
}

template<class Heap>
int extractMin(std::vector<Heap*> &heaps, int index)
{
	return heaps[index]->extractMin();
}

template<class Heap>
void meld(std::vector<Heap*> &heaps, int index1, int index2)
{
	heaps[index1]->meld(std::move(*heaps[index2]));
	assert(heaps[index2]->empty());
	std::swap(heaps[index2], heaps.back());
	heaps.pop_back();
}

enum OperationType
{
	ADD_HEAP,
	INSERT,
	EXTRACT_MIN,
	MELD
};

struct TestResult
{
	std::string name;
	size_t numberOperations;
	std::vector<int> mins;
	float time;

	TestResult(std::string name, size_t numberOperations)
			: name(name), numberOperations(numberOperations)
	{
	}
};

template<class Heap>
class Tester
{
private:
	const static size_t FIRST_TEST_NUMBER = 7777;
	const static size_t TEST_NUMBER = 50000;
	std::vector<Heap*> heaps;

	bool tryAddHeap()
	{
		//printf("AddHeap %d\n", key);
		addHeap(heaps, rand());
		return 1;
	}

	bool tryInsert()
	{
		if (heaps.empty())
			return 0;
		int index = rand() % heaps.size();
		int key = rand();
		//printf("Insert %d %d\n", index, key);
		insert(heaps, index, key);
		return 1;
	}

	std::pair<bool, int> tryExtractMin()
	{
		if (heaps.empty())
			return std::make_pair(false, 0);
		size_t index = rand() % heaps.size();
		if (heaps[index]->empty())
			return std::make_pair(false, 0);
		//printf("ExtractMin %d\n", index);
		int min = extractMin(heaps, index);
		return std::make_pair(true, min);
	}

	bool tryMeld()
	{
		if (heaps.size() < 2)
			return 0;
		size_t index1 = rand() % heaps.size();
		size_t index2 = rand() % heaps.size();
		if (index2 == index1)
			index2 = (index2 + 1) % heaps.size();
		//printf("Meld %d %d\n", index1, index2);
		meld(heaps, index1, index2);
		return 1;
	}

	std::vector<int> run(size_t number, int seed)
	{
		std::vector<int> mins;
		std::srand(seed);
		for (size_t i = 0; i < number;)
		{
			OperationType type = static_cast<OperationType>(rand() % 4);
			bool isOperationApply = 0;
			switch (type)
			{
				case ADD_HEAP:
					isOperationApply = tryAddHeap();
					break;
				case INSERT:
					isOperationApply = tryInsert();
					break;
				case EXTRACT_MIN:
					{
					auto result = tryExtractMin();
					isOperationApply = result.first;
					mins.push_back(result.second);
					break;
				}
				case MELD:
					isOperationApply = tryMeld();
					break;
			}
			if (isOperationApply)
				++i;
		}
		return mins;
	}

public:
	TestResult test(int seed, size_t maxNumber, std::string name)
	{
		for (int i = 0; i < 3; ++i)
			run(FIRST_TEST_NUMBER, seed);

		TestResult result(name, TEST_NUMBER);
		auto timeStart = std::chrono::high_resolution_clock::now();
		result.mins = run(TEST_NUMBER, seed);
		auto timeEnd = std::chrono::high_resolution_clock::now();
		result.time = std::chrono::duration_cast < std::chrono::microseconds > (timeEnd - timeStart).count() / 1e6;
		return result;
	}
};

bool checkEqual(std::vector<TestResult> results)
{
	printf("%d\n", results[0].mins.size());
	for (TestResult result : results)
		if (result.mins != results[0].mins)
			return false;
	return true;
}

void test(size_t maxNumber)
{
	int seed = time(0);

	std::vector<TestResult> results;
	results.push_back(Tester<PriorityQueue>().test(seed, maxNumber, "queue"));
	results.push_back(Tester<SetHeap>().test(seed, maxNumber, "set"));
	results.push_back(Tester<LeftHeap>().test(seed, maxNumber, "left"));
	results.push_back(Tester<SkewHeap>().test(seed, maxNumber, "skew"));
	results.push_back(Tester<BinomialHeap>().test(seed, maxNumber, "binomial"));

	assert(checkEqual(results));

	printf("%10s  |  ", "number");
	for (TestResult result : results)
		printf("%10s", result.name.c_str());
	printf("\n");

	printf("%10d  |  ", results[0].numberOperations);
	for (TestResult result : results)
		printf("%10.4f", result.time / results[0].time);
	printf("\n");
}

#endif
