#ifndef TEST_SORT
#define TEST_SORT

#include "externalSort.h"
#include "generator.h"

template<class T>
class VectorDataInput : public IDataInput<T>
{
public:
	VectorDataInput(std::vector<T> objects)
		: objects_(objects)
		, currentIndex_(0)
	{}
	
	virtual bool hasNext() const
	{
		return currentIndex_ < objects_.size();
	}
	
	virtual T get()
	{
		return objects_[currentIndex_++];
	}
	
	virtual ~VectorDataInput() {}
private:
	std::vector<T> objects_;
	size_t currentIndex_;
};

template<class T>
class ConstDataInput : public IDataInput<T>
{
public:
	ConstDataInput(T object, unsigned int number)
		: object_(object)
		, number_(number)
	{}
	
	virtual bool hasNext() const
	{
		return number_ > 0;
	}
	
	virtual T get()
	{
		assert(number_ > 0);
		--number_;
		return object_;
	}
	
	virtual ~ConstDataInput() {}
private:
	T object_;
	unsigned int number_;
};

template<class T>
class RandomDataInput : public IDataInput<T>
{
public:
	RandomDataInput(T (*generator)(), unsigned int number)
		: generator_(generator)
		, number_(number)
	{}
	
	virtual bool hasNext() const
	{
		return number_ > 0;
	}
	
	virtual T get()
	{
		assert(number_ > 0);
		--number_;
		return (*generator_)();
	}
	
	virtual ~RandomDataInput() {}
private:
	T (*generator_)();
	unsigned int number_;
};

template<class T>
class Checker : public IDataOutput<T>
{
public:
	virtual void put(const T &object)
	{
		if (isPrevObject_)
			assert(prevObject <= object);
		prevObject = object;
		isPrevObject_ = 1;
	}
	
	virtual void finish() {}
	
	virtual ~Checker() {}
private:
	bool isPrevObject_ = 0;
	T prevObject;
};

template<class T>
void test(IDataInput<T>* iDataInput, unsigned int maxNumber)
{
	IDataOutput<T>* checker = new Checker<T>();
	ExternalSort<T>* es = new ExternalSort<T>(iDataInput, checker, maxNumber);
	es->execute();
	delete checker;
	delete es;
}

template<class T, template<class T> class V>
void testOnSetMaxNumbers(V<T> iDataInput, std::vector<unsigned int> maxNumbers)
{
	for (unsigned int maxNumber : maxNumbers)
	{
		V<T> currentDataInput = iDataInput;
		test(&currentDataInput, maxNumber);
	}
}

void testSort()
{
	std::cout << "\tSort:" << std::endl;
	
	std::cout << "Small tests ints ... ";
	testOnSetMaxNumbers(VectorDataInput<int>({1}), {1});
	testOnSetMaxNumbers(VectorDataInput<int>({2, 1, 3}), {1, 2, 3});
	testOnSetMaxNumbers(VectorDataInput<int>({8, 5, 1, 7, 4, 9, 7, 6, 1, 9, 6, 5, 1}), {1, 2, 3, 5, 10});
	testOnSetMaxNumbers(VectorDataInput<int>({7, 7, 7, 7, 7, 7, 7, 7, 7}), {1, 2, 4, 7});
	std::cout << "OK" << std::endl;
	
	std::cout << "Small tests strings ... ";
	testOnSetMaxNumbers(VectorDataInput<std::string>({"a"}), {1});
	testOnSetMaxNumbers(VectorDataInput<std::string>({"abc", "aba", "abb"}), {1, 2, 3});
	testOnSetMaxNumbers(VectorDataInput<std::string>({"jcsdd", "acsbg", "yabk", " d7q ag ", "q w e r", "", "1", ":", "It's string!", "23.10.2015"}), {1, 2, 3, 5, 10});
	testOnSetMaxNumbers(VectorDataInput<std::string>({"sample", "sample", "sample", "sample", "sample", "sample", "sample", "sample", "sample", "sample"}), {1, 2, 4, 7});
	std::cout << "OK" << std::endl;
	
	std::cout << "Small tests vector<int>'s ... ";
	testOnSetMaxNumbers(VectorDataInput<std::vector<int>>({{0, 0}, {}}), {1});
	testOnSetMaxNumbers(VectorDataInput<std::vector<int>>({{2}, {1}, {3}}), {1, 2, 3});
	testOnSetMaxNumbers(VectorDataInput<std::vector<int>>({{1, 8}, {2, 5}, {1, 1}, {3, 7}, {2, 4}, {1, 9}, {2, 7}, {2, 6}, {3, 1}, {3, 9}, {2, 6}, {1, 5}, {1, 1}}), {1, 2, 3, 5, 10});
	testOnSetMaxNumbers(VectorDataInput<std::vector<int>>({{7, 77}, {7, 77}, {7, 77}, {7, 77}, {7, 77}, {7, 77}, {7, 77}, {7, 77}, {7, 77}, {7, 77}, {7, 77}, {7, 77}, {7, 77}}), {1, 2, 4, 7});
	std::cout << "OK" << std::endl;
	
	std::cout << "Big const ints test ... ";
	testOnSetMaxNumbers(ConstDataInput<int>(7, 100), {10, 20});
	testOnSetMaxNumbers(ConstDataInput<int>(77, 1000), {70, 100, 300});
	testOnSetMaxNumbers(ConstDataInput<int>(777, 10000), {700, 1000, 1024});
	testOnSetMaxNumbers(ConstDataInput<int>(7777, 100000), {2999, 3000, 3001});
	std::cout << "OK" << std::endl;
	
	std::cout << "Big const strings test ... ";
	testOnSetMaxNumbers(ConstDataInput<std::string>("7", 100), {10, 20});
	testOnSetMaxNumbers(ConstDataInput<std::string>("77", 1000), {70, 100, 300});
	testOnSetMaxNumbers(ConstDataInput<std::string>("kafcns hvgcw c ", 10000), {700, 1000, 1024});
	testOnSetMaxNumbers(ConstDataInput<std::string>("!", 100000), {2999, 3000, 3001});
	std::cout << "OK" << std::endl;
	
	std::cout << "Big random ints test ... ";
	return;
	testOnSetMaxNumbers(RandomDataInput<int>(generateInt, 10000), {10000000});
	testOnSetMaxNumbers(RandomDataInput<int>(generateInt, 100000), {30001});
	testOnSetMaxNumbers(RandomDataInput<int>(generateInt, 1000000), {100000});
	testOnSetMaxNumbers(RandomDataInput<int>(generateInt, 10000000), {345678});
	testOnSetMaxNumbers(RandomDataInput<int>(generateInt, 100000000), {10000000});
	testOnSetMaxNumbers(RandomDataInput<int>(generateInt, 1000000000), {10000000});
	std::cout << "OK" << std::endl;
}

#endif