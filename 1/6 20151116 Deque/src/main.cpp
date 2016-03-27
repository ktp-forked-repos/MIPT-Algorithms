#include <bits/stdc++.h>
#include "MyDeque.h"
#include "MyIterator.h"

#define dbg(x) std::cerr << #x << " = " << (x) << std::endl;

template<typename T>
void print(MyDeque<T> &d)
{
	std::for_each(d.begin(), d.end(), [](int x)
	{
		std::cout << x << " ";
	});
	std::cout << std::endl;
}

#include "operationType.h"

void run(std::string fileName, std::vector<OperationType> operations)
{
	FILE* f = fopen(("plots/" + fileName).c_str(), "w");
	fprintf(f, "x y\n");
	int i = 0;

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
		++i;
		if (i % 100 == 0)
			fprintf(f, "%d %d\n", i, d.size());
	}
}

/*
 int main()
 {
 srand(77);
 int number = 200000;
 run("Random.txt", RandomGenerator().get(number));
 //run("1.txt", FullIncreaseDecreaseCycle().get(number));
 //run("2.txt", FewFullIncreaseDecreaseCycles().get(number));
 run("PartIncreaseDecreaseCycle.txt", PartIncreaseDecreaseCycle().get(number));

 printf("OK\n");
 return 0;
 }
 //*/
