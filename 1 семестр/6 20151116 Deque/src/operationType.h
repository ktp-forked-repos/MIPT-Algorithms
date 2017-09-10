#ifndef SRC_OPERATIONTYPE_H_
#define SRC_OPERATIONTYPE_H_

enum OperationType
{
	PUSH_BACK,
	PUSH_FRONT,
	POP_BACK,
	POP_FRONT
};

class Generator
{
protected:
	static const size_t MEDIUM_SIZE = 1000;

	std::vector<OperationType> operations;
	size_t size = 0;

	void push(double pushBackProbability)
	{
		++size;
		operations.push_back((rand() % 100 < pushBackProbability * 100) ? PUSH_BACK : PUSH_FRONT);
	}

	void pop(double popBackProbability)
	{
		if (size == 0)
			return;
		--size;
		operations.push_back((rand() % 100 < popBackProbability * 100) ? POP_BACK : POP_FRONT);
	}

	void addRandomOperation()
	{
		if (rand() % 2)
			push(0.5);
		else
			pop(0.5);
	}

	void smoothChangeSize(size_t newSize, size_t operationsLimit)
	{
		size_t newOperationsSize = operations.size() + operationsLimit;
		double pushProbability = size < newSize ? 0.7 : 0.3;
		double pushBackProbability = (rand() % 2) ? 0.7 : 0.3;
		double popBackProbability = 1 - pushBackProbability;
		while (size != newSize && operations.size() < newOperationsSize)
			if (rand() % 100 < pushProbability * 100)
				push(pushBackProbability);
			else
				pop(popBackProbability);
	}

	void smoothIncrease(size_t number)
	{
		smoothChangeSize(size + number, INT_MAX);
	}

	void smoothDecrease(size_t number)
	{
		smoothChangeSize(size - std::min(size, number), INT_MAX);
	}

	virtual void generate(size_t numberOperations) = 0;

public:
	std::vector<OperationType> get(size_t numberOperations)
	{
		generate(numberOperations);
		return operations;
	}

	virtual ~Generator()
	{
	}
};

class RandomGenerator: public Generator
{
protected:
	virtual void generate(size_t numberOperations)
	{
		while (numberOperations--)
			addRandomOperation();
	}

public:
	virtual ~RandomGenerator()
	{
	}
};

class FullIncreaseDecreaseCycle: public Generator
{
protected:
	virtual void generate(size_t numberOperations)
	{
		size_t numberOperationsInPart = numberOperations / 2;
		smoothChangeSize(INT_MAX, numberOperationsInPart);
		smoothChangeSize(0, numberOperationsInPart);
	}

public:
	virtual ~FullIncreaseDecreaseCycle()
	{
	}
};

class FewFullIncreaseDecreaseCycles: public Generator
{
protected:
	virtual void generate(size_t numberOperations)
	{
		size_t parts = 5;
		size_t numberOperationsInPart = numberOperations / parts / 2;
		while (operations.size() < numberOperations)
		{
			smoothIncrease(numberOperationsInPart);
			smoothDecrease(numberOperationsInPart);
		}
	}

public:
	virtual ~FewFullIncreaseDecreaseCycles()
	{
	}
};

class PartIncreaseDecreaseCycle: public Generator
{
protected:
	virtual void generate(size_t numberOperations)
	{
		smoothIncrease(MEDIUM_SIZE);

		while (operations.size() < numberOperations)
			if (rand() % 100 != 0)
				addRandomOperation();
			else if (rand() % 5 == 0 && size < MEDIUM_SIZE / 5)
				smoothChangeSize(MEDIUM_SIZE, INT_MAX);
			else
			{
				if (rand() % 2 == 0 && size * 2 < MEDIUM_SIZE * 5)
					smoothIncrease(size / 2);
				else
					smoothDecrease(size / 2);
			}
	}

public:
	virtual ~PartIncreaseDecreaseCycle()
	{
	}
};

#endif
