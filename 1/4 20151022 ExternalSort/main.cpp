#include <bits/stdc++.h>

#include "iData.h"
#include "externalSort.h"
#include "externalReverse.h"
#include "testSerialize.h"
#include "testFiles.h"
#include "testSort.h"

class SimpleDataInput : public IDataInput<int>
{
public:
	SimpleDataInput(int n): n_(n)
	{}
	
	virtual int get()
	{
		int ret = rand() % 10;
		std::cout << ret << " ";
		--n_;
		return ret;
	}
	
	virtual bool hasNext() const
	{
		if (n_ == 0)
			std::cout << std::endl;
		return n_ > 0;
	}
	
	virtual ~SimpleDataInput() {};
private:
	int n_;
};

class SimpleDataOutput : public IDataOutput<int>
{
public:
	virtual void put(const int &object)
	{
		std::cout << object << " ";
	}
	
	virtual void finish()
	{
		std::cout << std::endl;
	}
	
	virtual ~SimpleDataOutput() {};
};

void exampleUsageSort()
{
	IDataInput<int>* iDataInput = new SimpleDataInput(15);
	IDataOutput<int>* iDataOutput = new SimpleDataOutput();
	
	ExternalSort<int>* externalAlgorithm = new ExternalSort<int>(iDataInput, iDataOutput, 4);
	externalAlgorithm->execute();
	
	delete externalAlgorithm;
	delete iDataInput;
	delete iDataOutput;
}

void exampleUsageReverse()
{
	IDataInput<int>* iDataInput = new SimpleDataInput(15);
	IDataOutput<int>* iDataOutput = new SimpleDataOutput();
	
	ExternalReverse<int>* externalAlgorithm = new ExternalReverse<int>(iDataInput, iDataOutput, 4);
	externalAlgorithm->execute();
	
	delete externalAlgorithm;
	delete iDataInput;
	delete iDataOutput;
}

int main()
{
	srand(time(0));
	testSerialize();
	testFiles();
	testSort();
	//exampleUsageSort();
	printf("%.3f seconds \n", clock() / (double) CLOCKS_PER_SEC);
	return 0;
}