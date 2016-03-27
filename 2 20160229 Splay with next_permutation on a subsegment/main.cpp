#include "base.h"
#include "iStruct.h"
#include "vectorStruct.h"
#include "splayStruct.h"

class IGenerator
{
  public:
	virtual void createRandomLR(size_t &l, size_t &r) = 0;
	virtual size_t getRandomI(bool isIndexInsert = false) = 0;
	virtual int getRandomValue() = 0;

	virtual ~IGenerator()
	{
	}
};

class AbstractOperation
{
  public:
	virtual int getSizeChange()
	{
		return 0;
	}

	virtual void perform(IStruct *s, std::vector<ll> &result) = 0;

	virtual ~AbstractOperation()
	{
	}
};

class AbstractOperationWithLR : public AbstractOperation
{
  protected:
	size_t l;
	size_t r;

	AbstractOperationWithLR(IGenerator *generator)
	{
		generator->createRandomLR(l, r);
	}
};

class AbstractOperationWithIndexAndValue : public AbstractOperation
{
  protected:
	size_t i;
	int x;

	AbstractOperationWithIndexAndValue(IGenerator *generator, bool isIndexInsert = false)
		: i(generator->getRandomI(isIndexInsert)), x(generator->getRandomValue())
	{
	}
};

class OperationSum : public AbstractOperationWithLR
{
  public:
	OperationSum(IGenerator *generator)
		: AbstractOperationWithLR(generator)
	{
	}

	virtual void perform(IStruct *s, std::vector<ll> &result)
	{
		result.push_back(s->getSum(l, r));
	}
};

class OperationInsert : public AbstractOperationWithIndexAndValue
{
  public:
	OperationInsert(IGenerator *generator)
		: AbstractOperationWithIndexAndValue(generator, true)
	{
	}

	virtual int getSizeChange()
	{
		return 1;
	}

	virtual void perform(IStruct *s, std::vector<ll> &result)
	{
		s->insert(i, x);
	}
};

class OperationSet : public AbstractOperationWithIndexAndValue
{
  public:
	OperationSet(IGenerator *generator)
		: AbstractOperationWithIndexAndValue(generator)
	{
	}

	virtual void perform(IStruct *s, std::vector<ll> &result)
	{
		s->set(i, x);
	}
};

class OperationAdd : public AbstractOperationWithLR
{
  private:
	int x;

  public:
	OperationAdd(IGenerator *generator)
		: AbstractOperationWithLR(generator), x(generator->getRandomValue())
	{
	}

	virtual void perform(IStruct *s, std::vector<ll> &result)
	{
		s->add(l, r, x);
	}
};

class OperationNextPermutation : public AbstractOperationWithLR
{
  public:
	OperationNextPermutation(IGenerator *generator)
		: AbstractOperationWithLR(generator)
	{
	}

	virtual void perform(IStruct *s, std::vector<ll> &result)
	{
		s->nextPermutation(l, r);
	}
};

class AbstractGenerator : public IGenerator
{
  public:
	size_t currentSize;
	size_t n;
	int rangeMin;
	int rangeMax;
	std::vector<AbstractOperation *> result;

	AbstractGenerator(size_t n = 10, int rangeMin = -10, int rangeMax = 10)
		: currentSize(0), n(n), rangeMin(rangeMin), rangeMax(rangeMax)
	{
	}

	virtual std::vector<AbstractOperation *> generate() = 0;

	void add(AbstractOperation *operation)
	{
		result.push_back(operation);
		currentSize += operation->getSizeChange();
	}

	virtual size_t getCurrentSize()
	{
		return currentSize;
	}

	virtual void createRandomLR(size_t &l, size_t &r)
	{
		l = getRandomI();
		r = getRandomI();
		if (l > r)
			std::swap(l, r);
	}

	virtual size_t getRandomI(bool isIndexInsert = false)
	{
		return rand() % (currentSize + isIndexInsert);
	}

	// [minRange, maxRange]
	virtual int getRandomValue()
	{
		return rangeMin + rand() % (rangeMax - rangeMin + 1);
	}

	virtual ~AbstractGenerator()
	{
	}
};

enum OperationType {
	GET_SUM,
	INSERT,
	SET,
	ADD,
	NEXT_PERMUTATION
};

class GeneratorRandom : public AbstractGenerator
{
  public:
	GeneratorRandom(size_t n = 10, int rangeMin = -10, int rangeMax = 10)
		: AbstractGenerator(n, rangeMin, rangeMax)
	{
	}

	virtual std::vector<AbstractOperation *> generate()
	{
		add(new OperationInsert(this));
		for (size_t i = 1; i < n; ++i) {
			OperationType type = static_cast<OperationType>(rand() % 5);
			switch (type) {
			case GET_SUM:
				add(new OperationSum(this));
				break;
			case INSERT:
				add(new OperationInsert(this));
				break;
			case SET:
				add(new OperationSet(this));
				break;
			case ADD:
				add(new OperationAdd(this));
				break;
			case NEXT_PERMUTATION:
				add(new OperationNextPermutation(this));
				break;
			default:
				assert(0);
			}
		}
		return result;
	}
};

std::vector<ll> run(IStruct *s, std::vector<AbstractOperation *> operations)
{
	std::vector<ll> result;
	for (AbstractOperation *operation : operations)
		operation->perform(s, result);
	return result;
}

void runTests()
{
	GeneratorRandom generator;
	std::vector<AbstractOperation *> operations = generator.generate();

	IStruct *s = new VectorStruct();
	run(s, operations);
	delete s;
}

int main()
{
	SplayStruct s;
	return 0;
}
