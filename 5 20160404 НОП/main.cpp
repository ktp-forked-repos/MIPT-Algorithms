#include <bits/stdc++.h>
#include "base.h"
#include "slow.h"
#include "fast.h"
#include "slowCheck.h"

std::string generateString(size_t n)
{
	std::string s(n, 'a');
	for (size_t i = 0; i < n; ++i)
		s[i] += rand() % 5;
	return s;
}

void generate(size_t n = 7, size_t m = 10)
{
	std::ofstream fout("input.txt");
	fout << generateString(n) << std::endl;
	fout << generateString(m) << std::endl;
	fout.close();
}

// возвращает число различающихся ячеек матриц
size_t diff(table t1, table t2)
{
	size_t diff = 0;
	assert(t1.size() == t2.size());
	for (size_t i = 0; i < t1.size(); ++i)
	{
		assert(t1[i].size() == t2[i].size());
		for (size_t j = 0; j < t1[i].size(); ++j)
			diff += t1[i][j] != t2[i][j];
	}
	return diff;
}

const char *spaces = "                    ";
void test(size_t numberTests = 20, size_t maxLength = 200)
{
	printf("Тест корректности\n");
	srand(time(0));
	size_t fail = 0;
	printf("Тестируем программу на %zu тестах...\n", numberTests);
	for (size_t i = 0; i < numberTests; ++i)
	{
		printf("Тест %zu", i);
		
		size_t n = rand() % maxLength + 1;
		size_t m = rand() % maxLength + 1;
		generate(n, m);
		
		freopen("input.txt", "r", stdin);
		std::string a, b;
		std::cin >> a >> b;
		table t1 = lcsSlow(a, b);
		table t2 = lcsFast(a, b);
		//assert(t1 == t2);
		
		printf("\r%s\r", spaces);
		if (t1 != t2)
		{
			printf("Fail:\n");
			printf("\ta=%s\n", a.c_str());
			printf("\tb=%s\n", a.c_str());
			printf("Таблицы различаются в %zu ячейках\n", diff(t1, t2));
			return;
		}
	}
	if (fail == 0)
		printf("Все тесты пройдены!\n");
}

void testTime(size_t length = 3000)
{
	printf("Тест времени работы\n");
	srand(time(0));
	generate(length, length);
	
	freopen("input.txt", "r", stdin);
	std::string a, b;
	std::cin >> a >> b;
	table t = lcsFast(a, b);
	printf("lcs строк длины %zu посчитался за %.3f секунд\n", length, clock() / float(CLOCKS_PER_SEC));
	printf("\n");
}

void testVisual(size_t n = 7, size_t m = 10)
{
	printf("Визуальный тест\n");
	srand(time(0));
	generate(n, m);
	
	freopen("input.txt", "r", stdin);
	std::string a, b;
	std::cin >> a >> b;
	table t = lcsFast(a, b);
	printf("lcs строк %s и %s:\n", a.c_str(), b.c_str());
	print(t);
}

int main()
{
	testVisual();
	testTime();
	test();
	return 0;
}
